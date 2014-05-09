/*
 *  RendererGL.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDERER_GL_H
#define _RENDERER_GL_H

#include "Matrix.h"
#include "Mesh.h"
#include "Material.h"
#include "Thread.h"
#include "Renderer.h"


#include <SDL2/SDL.h>
#ifdef TARGETIPHONE
#include <OpenGLES/ES1/gl.h>
#else
//#include "SDL_opengl.h"
#endif


//THIS CLASS SHOULD BE RENDER THREAD SAFE!!!!

class RendererGL : public Renderer
{
		
	public:
		RendererGL(int pWidth, int pHeight,unsigned int frameBufferID = 0,unsigned int renderBufferID = 0);
		~RendererGL();
		
		void Init();
		void Shutdown();
		void Render();
		void UseCamera(Camera* c);
		void SetupStringToEnumDB();
		
		virtual std::string GetTypeString()
		{
			return "RendererGL";
		}
		
	private:
		void gluPerspective(float fovy, float aspect, float zNear, float zFar);
		//virtual void RasterizeRenderTargets();
		virtual void SetupGPUState(Mesh& mesh,Material& mat);
		virtual void RestoreGPUState(Mesh& mesh,Material& mat);
		virtual void UploadTextureDataToGPU(Texture& tex);
		virtual void DeleteTextureFromGPU(Texture& tex);
		
	protected:
		void CreateFrameBufferObject(RenderTarget& rt){};
		int GetCurrrentFrameBufferObjectID(){ return 0;};
		int GetCurrrentRenderBufferObjectID(){return 0;};
		void BindFrameBuffer(int renderBufferID){};
		void BindRenderBuffer(int renderBufferID){};
	protected:
		/* SDL window */
		SDL_Window *mWindow;
		/* SDL GL context, binded to SDL window */
		SDL_GLContext mGLContext;

};



#endif //_RENDERER_GL_H