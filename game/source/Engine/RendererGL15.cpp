/*
 *  RendererGL15.cpp
 *  iPhone
 *
 *  Created by Kristofer Schlachter on 11/23/09.
 *  Copyright 2009 Gotham Wave Games, Inc. All rights reserved.
 *
 */

#include "RendererGL15.h"
#include "RenderTarget.h"


////////////////////////////////////////////////////////////////////////////////
void RendererGL15::CreateFrameBufferObject(RenderTarget& rt)
{
	GLint oldFBO;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &oldFBO);
	//GLint oldFBO_DEPTH;
	//glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &oldFBO_DEPTH);
	
	glGenTextures(1, (GLuint*)&rt.mTargetTexture->mHardwareID);
    glBindTexture(GL_TEXTURE_2D, rt.mTargetTexture->mHardwareID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, (GLuint)rt.mTargetTexture->mPixelFormat, rt.mTargetTexture->mWidth, rt.mTargetTexture->mHeight, 0, (GLuint)rt.mTargetTexture->mPixelFormat, GL_UNSIGNED_BYTE, rt.mTargetTexture->mPixelData);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR  );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	//Setup Texture Framebuffer
    glGenFramebuffersEXT(1, (GLuint*)&rt.mFBOID);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, rt.mFBOID);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rt.mTargetTexture->mHardwareID, 0);
	
	GLint backingWidth, backingHeight;
	glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_WIDTH_EXT, &backingWidth);
    glGetRenderbufferParameterivEXT(GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_HEIGHT_EXT, &backingHeight);
    
	glGenRenderbuffersEXT(1, (GLuint*)&rt.mFBO_DEPTHID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rt.mFBO_DEPTHID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT16, rt.mTargetTexture->mWidth, rt.mTargetTexture->mHeight);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rt.mFBO_DEPTHID);
	
	
	//Clear the texture bind
	//glBindTexture(GL_TEXTURE_2D,0);
	
	// check if it worked (probably worth doing :) )
	GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:						printf("GL_FRAMEBUFFER_COMPLETE_EXT\n");break;
		case 0x8CDB:										printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:			printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:	printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:			printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");break;			
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:					printf("GL_FRAMEBUFFER_UNSUPPORTED\n");break;	
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
////////////////////////////////////////////////////////////////////////////////
int RendererGL15::GetCurrrentFrameBufferObjectID()
{
	GLint fboid;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &fboid);
	return (int)fboid;
}
////////////////////////////////////////////////////////////////////////////////
int RendererGL15::GetCurrrentRenderBufferObjectID()
{
	GLint fboid;
	glGetIntegerv(GL_RENDERBUFFER_EXT, &fboid);
	return (int)fboid;
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL15::BindFrameBuffer(int buffID)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, (GLint)buffID);
	// check if it worked (probably worth doing :) )
	GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:						/*printf("GL_FRAMEBUFFER_COMPLETE_EXT\n")*/;break;
		case 0x8CDB:										printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:			printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:	printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:			printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n");break;			
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:					printf("GL_FRAMEBUFFER_UNSUPPORTED\n");break;	
		default:											printf("Unknown issue (%x).\n",status);break;	
	}
}
////////////////////////////////////////////////////////////////////////////////
void RendererGL15::BindRenderBuffer(int buffID)
{
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, (GLint)buffID);
}
////////////////////////////////////////////////////////////////////////////////
