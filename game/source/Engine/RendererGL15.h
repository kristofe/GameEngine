/*
 *  RendererGL.h
 *  XPlatformGL
 *
 *  Created by Kristofer Schlachter on 1/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDERER_GL15L_H
#define _RENDERER_GL15L_H

#include "RendererGL.h"



//THIS CLASS SHOULD BE RENDER THREAD SAFE!!!!

class RendererGL15 : public RendererGL
	{
		
	public:
		RendererGL15(int pWidth, int pHeight,unsigned int frameBufferID = 0,unsigned int renderBufferID = 0):
		RendererGL(pWidth,pHeight,frameBufferID,renderBufferID){};
		~RendererGL15(){};
		
		virtual std::string GetTypeString()
		{
			return "RendererGL15";
		}
		
		
		virtual void CreateFrameBufferObject(RenderTarget& rt);
		virtual int GetCurrrentFrameBufferObjectID();
		virtual int GetCurrrentRenderBufferObjectID();
		virtual void BindFrameBuffer(int renderBufferID);
		virtual void BindRenderBuffer(int renderBufferID);
		
	};



#endif //_RENDERER_GL_H