/*
 *  RendererGL.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDERER_GLES20_H
#define _RENDERER_GLES20_H

#include "Matrix.h"
#include "Mesh.h"
#include "Material.h"
#include "Thread.h"
#include "Renderer.h"


#include "SDL.h"
#ifdef TARGET_OS_IPHONE
#include <SDL_opengles.h>
#else
#include "SDL_opengl.h"
#endif


//THIS CLASS SHOULD BE RENDER THREAD SAFE!!!!

class RendererGLES20 : public Renderer
	{
		
	public:
		RendererGLES20(int pWidth, int pHeight,unsigned int frameBufferID = 0,unsigned int renderBufferID = 0);
		~RendererGLES20();
		
		void Init();
		void Shutdown();
		void Render();
		void SetWindowSize(int pWidth, int pHeight);
		void SetupStringToEnumDB();
		
		bool IsShaderCompilerAvailable();
		unsigned int LoadShader ( unsigned int type, const char *shaderSrc );
		unsigned int LoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );
		void GetActiveAttributeNamesAndLocations(unsigned int programObj, std::list<ShaderAttributeBinding>&  metaDataList);
		void GetActiveUniformNamesAndLocations(unsigned int programObj, std::list<ShaderUniformBinding>&  metaDataList);
		bool GetShaderCompilerInfo(bool hasCompiler, int* numBinaryFormats, int* formats);
		
	private:
		virtual void RasterizeRenderTargets();
		virtual void SetupGPUState(Mesh& mesh,Material& mat);
		virtual void RestoreGPUState(Mesh& mesh,Material& mat);
		virtual void UploadTextureDataToGPU(Texture& tex);
		virtual void DeleteTextureFromGPU(Texture& tex);
		
		virtual void BindShaderAndData(Mesh& mesh,Material& mat);
		
	protected:
		void CreateFrameBufferObject(RenderTarget& rt){};
		int GetCurrrentFrameBufferObjectID(){return 0;};
		int GetCurrrentRenderBufferObjectID(){return 0;};
		void BindFrameBuffer(int renderBufferID){};
		void BindRenderBuffer(int renderBufferID){};
		
	protected:
		/* SDL window */
		SDL_WindowID mWindow;
		/* SDL GL context, binded to SDL window */
		SDL_GLContext mGLContext;
		
		IvMatrix44		mProjectionMatrix;
		IvMatrix44*		mModelViewMatrix;
	};



#endif //_RENDERER_GL_H