#include <iostream>
#include "RendererGL.h"
#include "Game.h"
#include "RenderObject.h"
#include "RenderTarget.h"
#include "Camera.h"
#include "Material.h"
#include "Texture.h"
#include "Platform.h"
#include <algorithm>

#ifdef TARGET_OS_IPHONE
#define WINDOWPOS 0
#define glFrustum glFrustumf
#define glClearDepth glClearDepthf
#define glOrtho glOrthof
#else
#define WINDOWPOS 40

#endif
////////////////////////////////////////////////////////////////////////////////
RendererGL::RendererGL(int pWidth, int pHeight, unsigned int frameBufferID, unsigned int renderBufferID):Renderer(pWidth,pHeight,frameBufferID,renderBufferID)
{
	Init();
}
////////////////////////////////////////////////////////////////////////////////
RendererGL::~RendererGL()
{
	Shutdown();
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::gluPerspective(float fovy, float aspect, float zNear, float zFar)
{
	float xmin, xmax, ymin, ymax;
	
	ymax = zNear * tan(fovy * M_PI / 360.0f);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);

	
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::Init()
{
	int status = 0;
	SetupStringToEnumDB();
	#ifndef TARGET_OS_IPHONE
    //Sets up OpenGL ES double buffering and visual parameters 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
    mWindow=SDL_CreateWindow("Game Engine",
							WINDOWPOS, WINDOWPOS,
							mWidth, mHeight,
							SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
							//| SDL_WINDOW_BORDERLESS
							);
    if (mWindow==0)
    {
        fprintf(stderr, "Can't create mWindow: %s\n", SDL_GetError());
        exit(1);
    }
	
    mGLContext=SDL_GL_CreateContext(mWindow);
    if (mGLContext==NULL)
    {
        fprintf(stderr, "Can't create OpenGL ES context: %s\n", SDL_GetError());
        exit(1);
    }
	
    status=SDL_GL_MakeCurrent(mWindow, mGLContext);
    if (status<0)
    {
        fprintf(stderr, "Can't set current OpenGL ES context: %s\n", SDL_GetError());
        exit(1);
    }
	
    // Enable swap on VSYNC 
    SDL_GL_SetSwapInterval(1);
	#endif

	    /* Enable smooth shading */
    glShadeModel(GL_SMOOTH);
	
    /* Set the background black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
    /* Depth buffer setup */

	glClearDepth(1.0f);

    
	
    /* Enables Depth Testing */
    glEnable(GL_DEPTH_TEST);
	mZTestEnabled = true;
	glDepthMask(GL_TRUE);
	mColorArrayActive = false;
	mVertexArrayActive = false;
	mTextureCoordArrayActive = false;
	//mTexturingEnabled = false;
	mZWriteEnabled = true;
	mBlendEnabled = false;
	mBlendFuncA = -1;
	mBlendFuncB = -1;
	//mBoundTexture = -1;
	mColorMask = IvColor::white;
	mAlphaTestEnabled = false;
	mAlphaFunc = -1;
	mAlphaFuncValue = -1.0f;
	mMultiTexturingEnabled = false;
	mMaxTextureUnits = 2;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, (GLint*)&mMaxTextureUnits);
	
	for(int i = 0; i < MAX_TEXTURES_PER_MATERIAL; i++)
	{
		mTexturingEnabled[i] = false;
		mBoundTexture[i] = -1;
	}
	/* The Type Of Depth Test To Do */
    glDepthFunc(GL_LEQUAL);
	//glDepthFunc(GL_ALWAYS);
	
    /* Really Nice Perspective Calculations */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	UseCamera(NULL);
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::Shutdown()
{
	/* clean up the mWindow */
#ifndef TARGET_OS_IPHONE
    SDL_GL_DeleteContext(mGLContext);
    SDL_DestroyWindow(mWindow);
#endif
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::Render()
{
	HandlePendingRenderObjectRemovals();
	HandlePendingRendererObjectDeletions();	
	//HandlePendingRenderTargetRemovals();
	HandlePendingCameraRemovals();
	
	//RasterizeRenderTargets();
	


	
	//Sort the transparent render objects
	std::sort(mObjectVectors[TransparentObjectsArray]->begin(), mObjectVectors[TransparentObjectsArray]->end(), RenderObject::ZSortFunction);
	
	for(int camIdx = 0; camIdx < mCameras.size(); camIdx++)
	{
		Camera& c = *mCameras[camIdx];
		IvMatrix44& camMtx = c.mWorldInverseTransform;
		IvMatrix44 xform;
		
		UseCamera(&c);
		
		if(c.GetRenderTarget() != NULL)
		{
			if(c.GetRenderTarget()->GetTexture().mHardwareID == 0)
				CreateFrameBufferObject(*(c.GetRenderTarget()));//Attempt to create a FBO
			if(c.GetRenderTarget()->mFBOID != 0)//It succeeded so bind it and clear it.
			{	
				BindFrameBuffer(c.GetRenderTarget()->mFBOID);
			}
		}
		else
		{
			BindFrameBuffer(mFrameBufferID);
		}
		
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		
		//BUG IN glClear on iPhone. it respects the depth mask which i am guessing it shouldn't -HOLY COW WAS THIS A PAIN IN THE ASS TO FIND!!!!!
		glDepthMask(GL_TRUE);
		mZWriteEnabled = true;
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		
		for(int i = 0; i < (int)ObjectsArrayCount; i++)
		{
			int idx = 0;
			RenderObjectVectorIt sortedIt = mObjectVectors[i]->begin();
			while(sortedIt != mObjectVectors[i]->end())
			{
				RenderObject& ro = *(*sortedIt);
				if(ro.GetLayerID() == c.mLayerMaskID)
				{
					if(ro.IsVisible() && ro.IsActive())// && !ro.ExclusiveToRenderTarget())
					{
						glMatrixMode(GL_MODELVIEW);
						glPushMatrix();
						//glLoadIdentity();
						xform = camMtx * ro.mWorldTransform;
						float* mtx = xform.mV;//ro.mWorldTransform.mV;
						glLoadMatrixf((GLfloat*)mtx);
						for(int j = 0; j < (int)ro.mMeshes.size(); j++)
						{
							GameID meshID = ro.mMeshes[j];
							GameID matID = ro.mMaterials[j];
							Mesh& mesh = *(mMeshes[meshID]);
							Material& mat = *(mMaterials[matID]);
							
							SetupGPUState(mesh,mat);
							
							glDrawElements(GL_TRIANGLES,mesh.mTriangleCount*3,GL_UNSIGNED_SHORT,mesh.mTriangles);
							
							RestoreGPUState(mesh,mat);
						}
						
						glPopMatrix();
					}
				}
				++sortedIt;
				idx++;
			}
		}
	}
	
	BindRenderBuffer(mRenderBufferID);
	
	
    /* Gather our frames per second */
    mFrames++;
    
    unsigned int t=GetTicks();
    if (t-mT0>=1000)
    {
        float seconds=(t-mT0)/1000.0f;
        mFPS = mFrames/seconds;
        //printf("%d frames in %g seconds = %g FPS\n", mFrames, seconds, mFPS);
        mT0=t;
        mFrames=0;
    }
    
	#ifndef TARGET_OS_IPHONE
	SDL_GL_SwapWindow(mWindow);
	#endif
}
////////////////////////////////////////////////////////////////////////////////
/*
void RendererGL::RasterizeRenderTargets()
{
	//GLint oldFBO = GetCurrrentFrameBufferObjectID();
	//GLint oldFBO_DEPTH = GetCurrrentRenderBufferObjectID();
	IvMatrix44 transform;
	for(unsigned int rtIdx = 0; rtIdx < mRenderTargets.size(); rtIdx++)
	{
		
		RenderTarget* rt = mRenderTargets[rtIdx];
		
		glViewport(0, 0, (GLsizei)rt->GetWidth(), (GLsizei)rt->GetHeight());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, rt->GetWidth(), 0, rt->GetHeight(),-500,500);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		if(rt->GetTexture().mHardwareID == 0)
			CreateFrameBufferObject(*rt);//Attempt to create a FBO
		if(rt->mFBOID != 0)//It succeeded so bind it and clear it.
		{	
			BindFrameBuffer(rt->mFBOID);
		}
		
		glClearColor(0.0f,0.0f,0.0f,0.0f);			
		//BUG IN glClear it respects the depth mask -HOLY COW WAS THIS A PAIN IN THE ASS TO FIND!!!!!
		glDepthMask(GL_TRUE);
		mZWriteEnabled = true;
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();
		
		if(rt->RenderEverything())
		{
			std::sort(mObjectVectors[TransparentObjectsArray]->begin(),mObjectVectors[TransparentObjectsArray]->end(), RenderObject::ZSortFunction);
		}
		else
		{
			std::sort(rt->mObjectVectors[TransparentObjectsArray]->begin(),rt->mObjectVectors[TransparentObjectsArray]->end(), RenderObject::ZSortFunction);
		}
		
	
		for(int i = 0; i < (int)ObjectsArrayCount; i++)
		{
			int idx = 0;
			RenderObjectVectorIt sortedIt; 
			RenderObjectVectorIt endIt;
			if(rt->RenderEverything())
			{
				sortedIt = mObjectVectors[i]->begin();
				endIt = mObjectVectors[i]->end();
			}
			else
			{
				sortedIt = rt->mObjectVectors[i]->begin();
				endIt = rt->mObjectVectors[i]->end();
			}
			while(sortedIt != endIt)
			{
				RenderObject& ro = *(*sortedIt);
				if(ro.IsVisible() && ro.IsActive())
				{
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					//glLoadIdentity();
					//float* mtx = ro.mWorldTransform.mV;
					transform = ro.mWorldTransform * rt->mWorldTransform;
					float* mtx = transform.mV;
					glLoadMatrixf((GLfloat*)mtx);
					for(int j = 0; j < (int)ro.mMeshes.size(); j++)
					{
						GameID meshID = ro.mMeshes[j];
						GameID matID = ro.mMaterials[j];
						Mesh& mesh = *(mMeshes[meshID]);
						Material& mat = *(mMaterials[matID]);
						if(!mat.mUseRenderTarget)
						{
							SetupGPUState(mesh,mat);
							
							glDrawElements(GL_TRIANGLES,mesh.mTriangleCount*3,GL_UNSIGNED_SHORT,mesh.mTriangles);
							
							RestoreGPUState(mesh,mat);
						}
					}
					glPopMatrix();
				}
				++sortedIt;
				idx++;
			}
		}
		
		if(rt->mFBOID == 0)//We couldn't create and FBO -- it returned a zero when we tried to create one.  So do the slower CopyTexSubImage Method
		{
			if(rt->GetTexture().mHardwareID == 0)//Texture not created yet so we need to create it.... for render textures you can pass a NULL for the pixel data
				UploadTextureDataToGPU(rt->GetTexture());
			//update the variables that track the renderer's state
			mBoundTexture[0] = rt->GetTexture().mHardwareID;
			mTexturingEnabled[0] = true;
			glClientActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
		
			//Copy the back buffer into the RenderTarget!
			glBindTexture(GL_TEXTURE_2D, rt->GetTexture().mHardwareID);
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, rt->GetOffsetX(), rt->GetOffsetY(), rt->GetWidth(), rt->GetHeight());
		}
	}

	BindFrameBuffer(mFrameBufferID);//A no-op when there is no FBO support.
	
	UseCamera(mMainCamera);
	
	
}
 */
////////////////////////////////////////////////////////////////////////////////
void RendererGL::UseCamera(Camera* c)
{
	
	/*
    // Height / width ration 
    GLfloat ratio;
	GLfloat height = pHeight;
	GLfloat width = pWidth;
    // Protect against a divide by zero
    if (height==0)
    {
        height=1;
    }
	
    ratio=(GLfloat)width/(GLfloat)height;
	
    // Setup our viewport. 
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	
    // change to the projection matrix and set our viewing volume. 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Set our perspective
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	
    // Make sure we're chaning the model view and not the projection 
    glMatrixMode(GL_MODELVIEW);
	
    // Reset The View 
    glLoadIdentity();
	*/
	
	//mWidth = pWidth;
	//mHeight = pHeight;
	
	if(c != NULL)
	{
		/*if(c->mIsMain)
		{
			mWidth = c->mViewport.width;
			mHeight = c->mViewport.height;
		}*/
		
		glViewport((GLsizei)c->mViewport.x, (GLsizei)c->mViewport.y, (GLsizei)c->mViewport.width, (GLsizei)c->mViewport.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		if(c->mIsOrtho)
			glOrtho(c->mLeft, c->mRight, c->mBottom, c->mTop,c->mNear,c->mFar);
		else
			glFrustum(c->mLeft, c->mRight, c->mBottom, c->mTop,c->mNear,c->mFar);
			
	}else
	{
		mWidth = 320;
		mHeight = 480;
		glViewport((GLsizei)0, (GLsizei)0, (GLsizei)mWidth, (GLsizei)mHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		glOrtho(0, mWidth, 0, mHeight,-500,500);
		
	}
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	//glScalef(mWidth,mHeight,1);
	
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::SetupGPUState(Mesh& mesh, Material& material)
{

	if(!mColorArrayActive)
	{
		mColorArrayActive = true;
		glEnableClientState(GL_COLOR_ARRAY);
	}
	if(!mVertexArrayActive)
	{
		mVertexArrayActive = true;
		glEnableClientState(GL_VERTEX_ARRAY);
	}
	if(!mTextureCoordArrayActive)
	{
		mTextureCoordArrayActive = true;
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	glColorPointer(4, GL_FLOAT,0, mesh.mColors);
	glVertexPointer(3, GL_FLOAT,0, mesh.mVertices);	
	glTexCoordPointer(2, GL_FLOAT, 0, mesh.mUV);
	
		
	

	//BLENDING STATE MANAGEMENT
	if(material.mBlend != mBlendEnabled)
	{
		mBlendEnabled = material.mBlend;
		// Enable blending
		if(material.mBlend)
		{
			glEnable(GL_BLEND);	

		}
		else
		{
			glDisable(GL_BLEND);
		}
	}
	if(mBlendEnabled)
	{
		bool updateBlendFunc = false;
		if(mBlendFuncA != material.mBlendFuncA)
		{
			updateBlendFunc = true;
			mBlendFuncA = material.mBlendFuncA;
		}
		if(mBlendFuncB != material.mBlendFuncB)
		{
			updateBlendFunc = true;
			mBlendFuncB = material.mBlendFuncB;
		}
		if(updateBlendFunc)
		{
			glBlendFunc((GLenum)material.mBlendFuncA, (GLenum)material.mBlendFuncB);
		}
	}
	
	//ALPHA TEST STATE MANAGEMENT
	if(material.mUseAlphaTest != mAlphaTestEnabled)
	{
		mAlphaTestEnabled = material.mUseAlphaTest;
		// Enable blending
		if(material.mUseAlphaTest)
		{
			glEnable(GL_ALPHA_TEST);	
			
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
		}
	}
	if(mAlphaTestEnabled)
	{
		bool updateAlphaFunc = false;
		if(mAlphaFunc != material.mAlphaFunc)
		{
			updateAlphaFunc = true;
			mAlphaFunc = material.mAlphaFunc;
		}
		if(mAlphaFuncValue != material.mAlphaFuncTestValue)
		{
			updateAlphaFunc = true;
			mAlphaFuncValue = material.mAlphaFuncTestValue;
		}
		if(updateAlphaFunc)
		{
			glAlphaFunc((GLenum)material.mAlphaFunc, (GLfloat)material.mAlphaFuncTestValue);
		}
	}
	
	//TEXTURE STATE MANAGEMENT
	if(material.mTextureCount != 0)
	{
		glMatrixMode(GL_TEXTURE);
		for(int i = 0; i < material.mTextureCount; i++)
		{
			mTexturingEnabled[i] = true;
			glClientActiveTexture(GL_TEXTURE0 + i);
			glTexCoordPointer(2, GL_FLOAT, 0, mesh.GetUVsForTextureIdx(i));
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			
			glActiveTexture(GL_TEXTURE0 + i);
			
			Texture& tex = GetTexture(material.mTextureIDs[i]);
			if(tex.mHardwareID == 0)
			{
				UploadTextureDataToGPU(tex);
			}

			
			glBindTexture(GL_TEXTURE_2D, tex.mHardwareID);
			glEnable(GL_TEXTURE_2D);

			
			glLoadMatrixf((GLfloat*)material.mTextureMatrices[i].mV);
			
			if(material.mMultiTexturingEnabled)
			{
				CombinerCommands& cc = material.mCombinerCommands[i];
				for(unsigned int j = 0; j < cc.mCommands.size(); j++)
				{
					GLenum op0, op1;
					std::pair<unsigned int, unsigned int>& p = cc.mCommands[j];
					op0 = p.first;
					op1 = p.second;
					glTexEnvi(GL_TEXTURE_ENV,op0,op1);
							  
				}
			}
			else
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
			
		}
				
	}
	else
	{
		for(int i = 0; i < MAX_TEXTURES_PER_MATERIAL; i++)
		{
			if(mTexturingEnabled[i])
			{
				mTexturingEnabled[i] = false;
				glActiveTexture(GL_TEXTURE0 + i);
				glDisable(GL_TEXTURE_2D);
			}
		}
	}
	
	//DEPTH TESTING STATE MANAGEMENT
	if(material.mZTest != mZTestEnabled)
	{
		if(material.mZTest)
		{
			glEnable(GL_DEPTH_TEST);
			mZTestEnabled = true;
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			mZTestEnabled = false;
		}
	}
	
	//DEPTH WRITING STATE MANAGEMENT
	if(material.mZWrite != mZWriteEnabled)
	{
		if(material.mZWrite)
		{
			glDepthMask(GL_TRUE);
			mZWriteEnabled = true;
		}
		else
		{
			glDepthMask(GL_FALSE);
			mZWriteEnabled = false;
		}
	}

	if(mColorMask != material.mColorMask)
	{	
		mColorMask = material.mColorMask;
		glColorMask(material.mColorMask.r==1.0f,material.mColorMask.g==1.0f,material.mColorMask.b==1.0f,material.mColorMask.a==1.0f);//Turn on all colors by default
	}
	
	
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::RestoreGPUState(Mesh& mesh, Material& material)
{
	/*
	 glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	 
	if(mMultiTexturingEnabled == true)
	{
		for(int i = 1; i < mMaxTextureUnits; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glDisable(GL_TEXTURE_2D);
		}
		mMultiTexturingEnabled = false;
	}*/
	if(mColorMask != IvColor::white)
	{	
		glColorMask(1,1,1,1);//Turn on all colors by default
		mColorMask = IvColor::white;
	}
	for(int i = 1; i < mMaxTextureUnits; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::DeleteTextureFromGPU(Texture& tex)
{
	if(tex.mHardwareID != 0){
		//glAreTexturesResident() can be used to see if it is even in memory.
		//printf("Deleting %s from GPU\n",tex.mName.c_str());
		//GLuint textureHWID;
		glDeleteTextures( 1, (const GLuint*)&(tex.mHardwareID));
		tex.mHardwareID = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::UploadTextureDataToGPU(Texture& tex)
{
	if(tex.mHardwareID == 0){
		GLuint textureHWID;
		glGenTextures( 1, &textureHWID );
		tex.mHardwareID = (unsigned int)textureHWID;
		//glPrioritizeTextures()
	}
	glBindTexture( GL_TEXTURE_2D, (GLuint)tex.mHardwareID );
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR  );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	
	//if(tex.mIsLuminanceAlpha)
	//{
	//	glTexImage2D( GL_TEXTURE_2D,
	//		0,
	//		GL_LUMINANCE_ALPHA,
	//		tex.mWidth,
	//		tex.mHeight,
	//		0,
	//		GL_LUMINANCE_ALPHA,
	//		GL_UNSIGNED_BYTE,
	//		tex.mPixelData);

	//}
	//else
	//{
		//glPixelStorei( GL_UNPACK_ROW_LENGTH, conv->pitch / conv->format->BytesPerPixel );
	
		if(tex.mIsCompressed)
		{
#ifndef WIN32
			//Why doesn't windows support this????
			glCompressedTexImage2D( GL_TEXTURE_2D, 0, (GLuint)tex.mPixelFormat, tex.mWidth, tex.mHeight, 0, (GLuint)tex.mDataLength,(GLvoid*) tex.mPixelData);
#endif	
		}
		else
		{
			glTexImage2D( GL_TEXTURE_2D, 0, (GLuint)tex.mPixelFormat, tex.mWidth, tex.mHeight, 0, (GLuint)tex.mPixelFormat, GL_UNSIGNED_BYTE, tex.mPixelData);
		}
	/*
		if(tex.mNumberOfColors ==4)
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex.mWidth, tex.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.mPixelData);
		else
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, tex.mWidth, tex.mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.mPixelData);
	 */
		//glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
	//}
	tex.OnDataUploaded();
	
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL::SetupStringToEnumDB()
{
	//std::map<std::string, int>	mStringToEnumDB;


	// Boolean //
    mStringToEnumDB["FALSE"] = GL_FALSE;
    mStringToEnumDB["TRUE"] = GL_TRUE;
		
		// BeginMode //
    mStringToEnumDB["POINTS"] = GL_POINTS;
    mStringToEnumDB["LINES"] =  GL_LINES;
    mStringToEnumDB["LINE_LOOP"] =  GL_LINE_LOOP;
    mStringToEnumDB["LINE_STRIP"] =  GL_LINE_STRIP;
    mStringToEnumDB["TRIANGLES"] =  GL_TRIANGLES;
    mStringToEnumDB["TRIANGLE_STRIP"] =  GL_TRIANGLE_STRIP;
    mStringToEnumDB["TRIANGLE_FAN"] =  GL_TRIANGLE_FAN;
		
		// AlphaFunction //
    mStringToEnumDB["NEVER"] =  GL_NEVER;
    mStringToEnumDB["LESS"] =  GL_LESS;
    mStringToEnumDB["EQUAL"] =  GL_EQUAL;
    mStringToEnumDB["LEQUAL"] =  GL_LEQUAL;
    mStringToEnumDB["GREATER"] =  GL_GREATER;
    mStringToEnumDB["NOTEQUAL"] =  GL_NOTEQUAL;
    mStringToEnumDB["GEQUAL"] =  GL_GEQUAL;
    mStringToEnumDB["ALWAYS"] =  GL_ALWAYS;
		
	
	// BlendingFactorDest //
    mStringToEnumDB["ZERO"] =  GL_ZERO;
    mStringToEnumDB["ONE"] = GL_ONE;
    mStringToEnumDB["SRC_COLOR"] =  GL_SRC_COLOR;
    mStringToEnumDB["ONE_MINUS_SRC_COLOR"] = GL_ONE_MINUS_SRC_COLOR;
    mStringToEnumDB["SRC_ALPHA"] = GL_SRC_ALPHA;
    mStringToEnumDB["ONE_MINUS_SRC_ALPHA"] =  GL_ONE_MINUS_SRC_ALPHA;
    mStringToEnumDB["DST_ALPHA"] =  GL_DST_ALPHA;
    mStringToEnumDB["ONE_MINUS_DST_ALPHA"] =  GL_ONE_MINUS_DST_ALPHA;
		
		// BlendingFactorSrc //

    mStringToEnumDB["DST_COLOR"] = GL_DST_COLOR;
    mStringToEnumDB["ONE_MINUS_DST_COLOR"] =  GL_ONE_MINUS_DST_COLOR;
    mStringToEnumDB["SRC_ALPHA_SATURATE"] =  GL_SRC_ALPHA_SATURATE;

	/*	
		// ClipPlaneName //
    mStringToEnumDB[" GL_CLIP_PLANE0                    0x3000
    mStringToEnumDB[" GL_CLIP_PLANE1                    0x3001
    mStringToEnumDB[" GL_CLIP_PLANE2                    0x3002
    mStringToEnumDB[" GL_CLIP_PLANE3                    0x3003
    mStringToEnumDB[" GL_CLIP_PLANE4                    0x3004
    mStringToEnumDB[" GL_CLIP_PLANE5                    0x3005
	*/

		
		// CullFaceMode //
    mStringToEnumDB["FRONT"] =  GL_FRONT;
    mStringToEnumDB["BACK"] = GL_BACK;
    mStringToEnumDB["FRONT_AND_BACK"] =  GL_FRONT_AND_BACK;
		
	
		// EnableCap //
    mStringToEnumDB["FOG"] = GL_FOG;
    mStringToEnumDB["LIGHTING"] = GL_LIGHTING;
    mStringToEnumDB["TEXTURE_2D"] = GL_TEXTURE_2D;
    mStringToEnumDB["CULL_FACE"] = GL_CULL_FACE;
    mStringToEnumDB["ALPHA_TEST"] = GL_ALPHA_TEST;
    mStringToEnumDB["BLEND"] = GL_BLEND;
    mStringToEnumDB["COLOR_LOGIC_OP"] = GL_COLOR_LOGIC_OP;
    mStringToEnumDB["DITHER"] = GL_DITHER;
    mStringToEnumDB["STENCIL_TEST"] = GL_STENCIL_TEST;
    mStringToEnumDB["DEPTH_TEST"] = GL_DEPTH_TEST;
	/*
		//      GL_LIGHT0 //
		//      GL_LIGHT1 //
		//      GL_LIGHT2 //
		//      GL_LIGHT3 //
		//      GL_LIGHT4 //
		//      GL_LIGHT5 //
		//      GL_LIGHT6 //
		//      GL_LIGHT7 //
    mStringToEnumDB[" GL_POINT_SMOOTH                   0x0B10
    mStringToEnumDB[" GL_LINE_SMOOTH                    0x0B20
    mStringToEnumDB[" GL_SCISSOR_TEST                   0x0C11
    mStringToEnumDB[" GL_COLOR_MATERIAL                 0x0B57
    mStringToEnumDB[" GL_NORMALIZE                      0x0BA1
    mStringToEnumDB[" GL_RESCALE_NORMAL                 0x803A
    mStringToEnumDB[" GL_POLYGON_OFFSET_FILL            0x8037
    mStringToEnumDB[" GL_VERTEX_ARRAY                   0x8074
    mStringToEnumDB[" GL_NORMAL_ARRAY                   0x8075
    mStringToEnumDB[" GL_COLOR_ARRAY                    0x8076
    mStringToEnumDB[" GL_TEXTURE_COORD_ARRAY            0x8078
    mStringToEnumDB[" GL_MULTISAMPLE                    0x809D
    mStringToEnumDB[" GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
    mStringToEnumDB[" GL_SAMPLE_ALPHA_TO_ONE            0x809F
    mStringToEnumDB[" GL_SAMPLE_COVERAGE                0x80A0
		
		// ErrorCode //
    mStringToEnumDB[" GL_NO_ERROR                       0
    mStringToEnumDB[" GL_INVALID_ENUM                   0x0500
    mStringToEnumDB[" GL_INVALID_VALUE                  0x0501
    mStringToEnumDB[" GL_INVALID_OPERATION              0x0502
    mStringToEnumDB[" GL_STACK_OVERFLOW                 0x0503
    mStringToEnumDB[" GL_STACK_UNDERFLOW                0x0504
    mStringToEnumDB[" GL_OUT_OF_MEMORY                  0x0505
	*/
		// FogMode //
		//      GL_LINEAR //
    mStringToEnumDB["EXP"] = GL_EXP;
    mStringToEnumDB["EXP2"] =  GL_EXP2;
		
		// FogParameter //
    mStringToEnumDB["FOG_DENSITY"] = GL_FOG_DENSITY;
    mStringToEnumDB["FOG_START"] = GL_FOG_START;
    mStringToEnumDB["FOG_END"] = GL_FOG_END;
    mStringToEnumDB["FOG_MODE"] =  GL_FOG_MODE;
    mStringToEnumDB["FOG_COLOR"] = GL_FOG_COLOR;
		
		// FrontFaceDirection //
    mStringToEnumDB["CW"] = GL_CW;
    mStringToEnumDB["CCW"] =  GL_CCW;

		// LightParameter //
    mStringToEnumDB["AMBIENT"] = GL_AMBIENT;
    mStringToEnumDB["DIFFUSE"] = GL_DIFFUSE;
    mStringToEnumDB["SPECULAR"] = GL_SPECULAR;
    mStringToEnumDB["POSITION"] = GL_POSITION;
    mStringToEnumDB["SPOT_DIRECTION"] = GL_SPOT_DIRECTION;
    mStringToEnumDB["SPOT_EXPONENT"] =  GL_SPOT_EXPONENT;
    mStringToEnumDB["SPOT_CUTOFF"] = GL_SPOT_CUTOFF;
    mStringToEnumDB["CONSTANT_ATTENUATION"] = GL_CONSTANT_ATTENUATION;
    mStringToEnumDB["LINEAR_ATTENUATION"] = GL_LINEAR_ATTENUATION;
    mStringToEnumDB["QUADRATIC_ATTENUATION"] = GL_QUADRATIC_ATTENUATION;
		
		// DataType //
    mStringToEnumDB["BYTE"] = GL_BYTE;
    mStringToEnumDB["UNSIGNED_BYTE"] = GL_UNSIGNED_BYTE;
    mStringToEnumDB["SHORT"] = GL_SHORT;
    mStringToEnumDB["UNSIGNED_SHORT"] = GL_UNSIGNED_SHORT;
    mStringToEnumDB["FLOAT"] = GL_FLOAT;
#ifdef GL_FIXED
    mStringToEnumDB["FIXED"] = GL_FIXED;
#endif
	 
		// LogicOp //
    mStringToEnumDB["CLEAR"] = GL_CLEAR;
    mStringToEnumDB["AND"] =  GL_AND;
    mStringToEnumDB["AND_REVERSE"] = GL_AND_REVERSE;
    mStringToEnumDB["COPY"] = GL_COPY;
    mStringToEnumDB["AND_INVERTED"] = GL_AND_INVERTED;
    mStringToEnumDB["NOOP"] = GL_NOOP;
    mStringToEnumDB["XOR"] = GL_XOR;
    mStringToEnumDB["OR"] = GL_OR;
    mStringToEnumDB["NOR"] = GL_NOR;
    mStringToEnumDB["EQUIV"] = GL_EQUIV;
    mStringToEnumDB["INVERT"] = GL_INVERT;
    mStringToEnumDB["OR_REVERSE"] = GL_OR_REVERSE;
    mStringToEnumDB["COPY_INVERTED"] = GL_COPY_INVERTED;
    mStringToEnumDB["OR_INVERTED"] = GL_OR_INVERTED;
    mStringToEnumDB["NAND"] = GL_NAND;
    mStringToEnumDB["SET"] = GL_SET;
		
		// MaterialFace //
		//      GL_FRONT_AND_BACK //
		
		// MaterialParameter //
    mStringToEnumDB["EMISSION"] =  GL_EMISSION;
    mStringToEnumDB["SHININESS"] = GL_SHININESS;
    mStringToEnumDB["AMBIENT_AND_DIFFUSE"] = GL_AMBIENT_AND_DIFFUSE;
		//      GL_AMBIENT //
		//      GL_DIFFUSE //
		//      GL_SPECULAR //
	
	
		// MatrixMode //
    mStringToEnumDB["MODELVIEW"] =  GL_MODELVIEW;
    mStringToEnumDB["PROJECTION"] = GL_PROJECTION;
    mStringToEnumDB["TEXTURE"] =  GL_TEXTURE;
	/*	
		// NormalPointerType //
		//      GL_BYTE //
		//      GL_SHORT //
		//      GL_FLOAT //
		//      GL_FIXED //
	*/
		// PixelFormat //
    mStringToEnumDB["ALPHA"] = GL_ALPHA;
	mStringToEnumDB["RGB"] =  GL_RGB;
    mStringToEnumDB["RGBA"] = GL_RGBA;
	mStringToEnumDB["LUMINANCE"] = GL_LUMINANCE;
    mStringToEnumDB["LUMINANCE_ALPHA"] =  GL_LUMINANCE_ALPHA;
		
		// PixelStoreParameter //
	mStringToEnumDB["UNPACK_ALIGNMENT"] =  GL_UNPACK_ALIGNMENT;
    mStringToEnumDB["PACK_ALIGNMENT"] = GL_PACK_ALIGNMENT;
		
	/*	// PixelType //
		//      GL_UNSIGNED_BYTE //
    mStringToEnumDB[" GL_UNSIGNED_SHORT_4_4_4_4         0x8033
    mStringToEnumDB[" GL_UNSIGNED_SHORT_5_5_5_1         0x8034
    mStringToEnumDB[" GL_UNSIGNED_SHORT_5_6_5           0x8363
	*/	
		// ShadingModel //
    mStringToEnumDB["FLAT"] = GL_FLAT;
	mStringToEnumDB["SMOOTH"] = GL_SMOOTH;
		
		// StencilFunction //
		//      GL_NEVER //
		//      GL_LESS //
		//      GL_EQUAL //
		//      GL_LEQUAL //
		//      GL_GREATER //
		//      GL_NOTEQUAL //
		//      GL_GEQUAL //
		//      GL_ALWAYS //
		
		// StencilOp //
    mStringToEnumDB["KEEP"] = GL_KEEP;
    mStringToEnumDB["REPLACE"] = GL_REPLACE;
    mStringToEnumDB["INCR"] = GL_INCR;
    mStringToEnumDB["DECR"] = GL_DECR;
		//      GL_INVERT //
		
		// StringName //
    mStringToEnumDB["VENDOR"] = GL_VENDOR;
    mStringToEnumDB["RENDERER"] = GL_RENDERER;
    mStringToEnumDB["VERSION"] = GL_VERSION;
    mStringToEnumDB["EXTENSIONS"] = GL_EXTENSIONS;

		
		// TextureEnvMode //
    mStringToEnumDB["MODULATE"] = GL_MODULATE;
    mStringToEnumDB["DECAL"] = GL_DECAL;
		//      GL_BLEND //
    mStringToEnumDB["ADD"] = GL_ADD;
		//      GL_REPLACE //
		
		// TextureEnvParameter //
    mStringToEnumDB["TEXTURE_ENV_MODE"] = GL_TEXTURE_ENV_MODE;
    mStringToEnumDB["TEXTURE_ENV_COLOR"] = GL_TEXTURE_ENV_COLOR;
		
		// TextureEnvTarget //
    mStringToEnumDB["TEXTURE_ENV"] = GL_TEXTURE_ENV;
		
	/*	// TextureMagFilter //
    mStringToEnumDB[" GL_NEAREST                        0x2600
    mStringToEnumDB[" GL_LINEAR                         0x2601

    mStringToEnumDB[" GL_LIGHT0                         0x4000
    mStringToEnumDB[" GL_LIGHT1                         0x4001
    mStringToEnumDB[" GL_LIGHT2                         0x4002
    mStringToEnumDB[" GL_LIGHT3                         0x4003
    mStringToEnumDB[" GL_LIGHT4                         0x4004
    mStringToEnumDB[" GL_LIGHT5                         0x4005
    mStringToEnumDB[" GL_LIGHT6                         0x4006
    mStringToEnumDB[" GL_LIGHT7                         0x4007
		
		// Buffer Objects //
    mStringToEnumDB[" GL_ARRAY_BUFFER                   0x8892
    mStringToEnumDB[" GL_ELEMENT_ARRAY_BUFFER           0x8893
		
    mStringToEnumDB[" GL_ARRAY_BUFFER_BINDING           0x8894
    mStringToEnumDB[" GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
    mStringToEnumDB[" GL_VERTEX_ARRAY_BUFFER_BINDING    0x8896
    mStringToEnumDB[" GL_NORMAL_ARRAY_BUFFER_BINDING    0x8897
    mStringToEnumDB[" GL_COLOR_ARRAY_BUFFER_BINDING     0x8898
    mStringToEnumDB[" GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
		
    mStringToEnumDB[" GL_STATIC_DRAW                    0x88E4
    mStringToEnumDB[" GL_DYNAMIC_DRAW                   0x88E8
		
    mStringToEnumDB[" GL_BUFFER_SIZE                    0x8764
    mStringToEnumDB[" GL_BUFFER_USAGE                   0x8765
	*/
		// Texture combine + dot3 //
    mStringToEnumDB["SUBTRACT"] = GL_SUBTRACT;
    mStringToEnumDB["COMBINE"] = GL_COMBINE;
    mStringToEnumDB["COMBINE_RGB"] = GL_COMBINE_RGB;
    mStringToEnumDB["COMBINE_ALPHA"] = GL_COMBINE_ALPHA;
    mStringToEnumDB["RGB_SCALE"] = GL_RGB_SCALE;
    mStringToEnumDB["ADD_SIGNED"] = GL_ADD_SIGNED;
    mStringToEnumDB["INTERPOLATE"] = GL_INTERPOLATE;
    mStringToEnumDB["CONSTANT"] = GL_CONSTANT;
    mStringToEnumDB["PRIMARY_COLOR"] = GL_PRIMARY_COLOR;
    mStringToEnumDB["PREVIOUS"] = GL_PREVIOUS;
    mStringToEnumDB["OPERAND0_RGB"] = GL_OPERAND0_RGB;
    mStringToEnumDB["OPERAND1_RGB"] = GL_OPERAND1_RGB;
    mStringToEnumDB["OPERAND2_RGB"] = GL_OPERAND2_RGB;
    mStringToEnumDB["OPERAND0_ALPHA"] = GL_OPERAND0_ALPHA;
    mStringToEnumDB["OPERAND1_ALPHA"] = GL_OPERAND1_ALPHA;
    mStringToEnumDB["OPERAND2_ALPHA"] = GL_OPERAND2_ALPHA;
		
    mStringToEnumDB["ALPHA_SCALE"] = GL_ALPHA_SCALE;
		
    mStringToEnumDB["SRC0_RGB"] = GL_SRC0_RGB;
    mStringToEnumDB["SRC1_RGB"] = GL_SRC1_RGB;
    mStringToEnumDB["SRC2_RGB"] = GL_SRC2_RGB;
    mStringToEnumDB["SRC0_ALPHA"] = GL_SRC0_ALPHA;
    mStringToEnumDB["SRC1_ALPHA"] = GL_SRC1_ALPHA;
    mStringToEnumDB["SRC2_ALPHA"] = GL_SRC2_ALPHA;
	
	mStringToEnumDB["SOURCE0_RGB"] = GL_SRC0_RGB;
    mStringToEnumDB["SOURCE1_RGB"] = GL_SRC1_RGB;
    mStringToEnumDB["SOURCE2_RGB"] = GL_SRC2_RGB;
    mStringToEnumDB["SOURCE0_ALPHA"] = GL_SRC0_ALPHA;
    mStringToEnumDB["SOURCE1_ALPHA"] = GL_SRC1_ALPHA;
    mStringToEnumDB["SOURCE2_ALPHA"] = GL_SRC2_ALPHA;
	
	
    mStringToEnumDB["DOT3_RGB"] = GL_DOT3_RGB;
    mStringToEnumDB["DOT3_RGBA"] = GL_DOT3_RGBA;
		
	mStringToEnumDB["TEXTURE0"] = GL_TEXTURE0;
	mStringToEnumDB["TEXTURE1"] = GL_TEXTURE1;
	mStringToEnumDB["TEXTURE2"] = GL_TEXTURE2;
	mStringToEnumDB["TEXTURE3"] = GL_TEXTURE3;
	mStringToEnumDB["TEXTURE4"] = GL_TEXTURE4;
	mStringToEnumDB["TEXTURE5"] = GL_TEXTURE5;
	mStringToEnumDB["TEXTURE6"] = GL_TEXTURE6;
	mStringToEnumDB["TEXTURE7"] = GL_TEXTURE7;
	mStringToEnumDB["TEXTURE8"] = GL_TEXTURE8;
	

	/*
					
		// OES_draw_texture //
    mStringToEnumDB[" GL_TEXTURE_CROP_RECT_OES          0x8B9D
		
		// OES_matrix_get //
    mStringToEnumDB[" GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES   0x898D
    mStringToEnumDB[" GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES  0x898E
    mStringToEnumDB[" GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES     0x898F
		
		// OES_matrix_palette //
    mStringToEnumDB[" GL_MAX_VERTEX_UNITS_OES           0x86A4
    mStringToEnumDB[" GL_MAX_PALETTE_MATRICES_OES       0x8842
    mStringToEnumDB[" GL_MATRIX_PALETTE_OES             0x8840
    mStringToEnumDB[" GL_MATRIX_INDEX_ARRAY_OES         0x8844
    mStringToEnumDB[" GL_WEIGHT_ARRAY_OES               0x86AD
    mStringToEnumDB[" GL_CURRENT_PALETTE_MATRIX_OES     0x8843
		
    mStringToEnumDB[" GL_MATRIX_INDEX_ARRAY_SIZE_OES    0x8846
    mStringToEnumDB[" GL_MATRIX_INDEX_ARRAY_TYPE_OES    0x8847
    mStringToEnumDB[" GL_MATRIX_INDEX_ARRAY_STRIDE_OES  0x8848
    mStringToEnumDB[" GL_MATRIX_INDEX_ARRAY_POINTER_OES 0x8849
    mStringToEnumDB[" GL_MATRIX_INDEX_ARRAY_BUFFER_BINDING_OES 0x8B9E
		
    mStringToEnumDB[" GL_WEIGHT_ARRAY_SIZE_OES          0x86AB
    mStringToEnumDB[" GL_WEIGHT_ARRAY_TYPE_OES          0x86A9
    mStringToEnumDB[" GL_WEIGHT_ARRAY_STRIDE_OES        0x86AA
    mStringToEnumDB[" GL_WEIGHT_ARRAY_POINTER_OES       0x86AC
    mStringToEnumDB[" GL_WEIGHT_ARRAY_BUFFER_BINDING_OES 0x889E
		
		// OES_point_size_array //
    mStringToEnumDB[" GL_POINT_SIZE_ARRAY_OES           0x8B9C
    mStringToEnumDB[" GL_POINT_SIZE_ARRAY_TYPE_OES      0x898A
    mStringToEnumDB[" GL_POINT_SIZE_ARRAY_STRIDE_OES    0x898B
    mStringToEnumDB[" GL_POINT_SIZE_ARRAY_POINTER_OES   0x898C
    mStringToEnumDB[" GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES 0x8B9F
		
		// OES_point_sprite //
    mStringToEnumDB[" GL_POINT_SPRITE_OES               0x8861
    mStringToEnumDB[" GL_COORD_REPLACE_OES              0x8862

	*/
	
}

////////////////////////////////////////////////////////////////////////////////
