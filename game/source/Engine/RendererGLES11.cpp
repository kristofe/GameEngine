/*
 *  RendererGLES11.cpp
 *  iPhone
 *
 *  Created by Kristofer Schlachter on 10/15/09.
 *  Copyright 2009 Gotham Wave Games, Inc. All rights reserved.
 *
 */
#include "RenderTarget.h"
#include "RendererGLES11.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

void RendererGLES11::CreateFrameBufferObject(RenderTarget& rt)
{
	GLint oldFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &oldFBO);
	//GLint oldFBO_DEPTH;
	//glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &oldFBO_DEPTH);
	
	glGenTextures(1, &rt.mTargetTexture->mHardwareID);
    glBindTexture(GL_TEXTURE_2D, rt.mTargetTexture->mHardwareID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, (GLuint)rt.mTargetTexture->mPixelFormat, rt.mTargetTexture->mWidth, rt.mTargetTexture->mHeight, 0, (GLuint)rt.mTargetTexture->mPixelFormat, GL_UNSIGNED_BYTE, rt.mTargetTexture->mPixelData);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR  );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	//Setup Texture Framebuffer
    glGenFramebuffersOES(1, &rt.mFBOID);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, rt.mFBOID);
    glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, rt.mTargetTexture->mHardwareID, 0);
	
	GLint backingWidth, backingHeight;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
	glGenRenderbuffersOES(1, &rt.mFBO_DEPTHID);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, rt.mFBO_DEPTHID);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, rt.mTargetTexture->mWidth, rt.mTargetTexture->mHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, rt.mFBO_DEPTHID);
	
	
	//Clear the texture bind
	//glBindTexture(GL_TEXTURE_2D,0);
	
	// check if it worked (probably worth doing :) )
	GLuint status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE_OES:						printf("GL_FRAMEBUFFER_COMPLETE_OES\n");break;
		case 0x8CDB:										printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES:			printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES:	printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES:			printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");break;			
		case GL_FRAMEBUFFER_UNSUPPORTED_OES:					printf("GL_FRAMEBUFFER_UNSUPPORTED\n");break;	
		default:											printf("Unknown issue (%x).\n",status);break;	
	}
	
	//status = glCheckFramebufferStatusOES(GL_RENDERBUFFER_OES);
	//if (status != GL_FRAMEBUFFER_COMPLETE_OES)
	//{
	//	printf("RENDER BUFFER(DEPTH PART) OBJECT ISN'T SETUP CORRECTLY\n");
	//}
    //glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFBO);
	//glBindRenderbufferOES(GL_RENDERBUFFER_OES, oldFBO_DEPTH);
    //glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    //glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
	//glGenRenderbuffersOES(1, &texDepthbuffer);
	//glBindRenderbufferOES(GL_RENDERBUFFER_OES, texDepthbuffer);
	//glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, 128, 128);
	//glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, texDepthbuffer);
    
    //if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
    //    NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    //    return NO;
    //}
    
}

int RendererGLES11::GetCurrrentFrameBufferObjectID()
{
	GLint fboid;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &fboid);
	return (int)fboid;
}

int RendererGLES11::GetCurrrentRenderBufferObjectID()
{
	GLint fboid;
	glGetIntegerv(GL_RENDERBUFFER_OES, &fboid);
	return (int)fboid;
}

void RendererGLES11::BindFrameBuffer(int buffID)
{
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, (GLint)buffID);
	// check if it worked (probably worth doing :) )
	GLuint status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE_OES:						/*printf("GL_FRAMEBUFFER_COMPLETE_OES\n")*/;break;
		case 0x8CDB:										printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES:			printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES:	printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES:			printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");break;			
		case GL_FRAMEBUFFER_UNSUPPORTED_OES:					printf("GL_FRAMEBUFFER_UNSUPPORTED\n");break;	
		default:											printf("Unknown issue (%x).\n",status);break;	
	}
}

void RendererGLES11::BindRenderBuffer(int buffID)
{
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, (GLint)buffID);
}
