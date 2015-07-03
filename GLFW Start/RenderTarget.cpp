#pragma once
#include "RenderTarget.h"
#include <assert.h>
#include <stdlib.h>

RenderTarget::RenderTarget(unsigned int width, unsigned int height, unsigned int mask) : m_fboHandle(0), m_renderTex2DHandle(-1), m_depthBufHandle(-1), m_drawBufs(new GLenum[1])
{
	m_drawBufs = new GLenum[1];

	// Generate and bind the framebuffer 
	glGenFramebuffers(1, &m_fboHandle); 
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
}

inline bool RenderTarget::IsValidMask( unsigned int mask) 
{
	return mask >= RenderTargetTextureCoponents::Color  && mask < RenderTargetTextureCoponents::Invalid;
}

//TODO: Add error handling. (Adding default texture to know if something went wrong, etc.)
void RenderTarget::initAndBindRenderTargetTextures(unsigned int width, unsigned int height, unsigned int mask)
{
	assert(IsValidMask(mask));

	if((RenderTargetTextureCoponents::Color & mask) == RenderTargetTextureCoponents::Color)
	{
		glGenTextures(1, &m_renderTex2DHandle);
		//glActiveTexture(GL_TEXTURE0);  // Use texture unit 0 
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, m_renderTex2DHandle);
 
		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
 
		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTex2DHandle, 0);
	}

	if((RenderTargetTextureCoponents::Depth & mask) == RenderTargetTextureCoponents::Depth)
	{
		glGenRenderbuffers(1, &m_depthBufHandle);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufHandle);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		// Bind the depth buffer to the FBO 
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufHandle); 
	}

	// Init default texture after
	//initDefaultTexture();

	renderBuffer();
}

RenderTarget::~RenderTarget()
{
	//glDeleteFramebuffers(1, m_fboHandle);
}

void RenderTarget::initDefaultTexture()
{
	// One pixel white texture 
	GLuint whiteTexHandle; 
	GLubyte whiteTex[] = { 255, 0, 255, 255 }; 
	glActiveTexture( GL_TEXTURE0 ); 
	glGenTextures( 1, &whiteTexHandle ); 
	glBindTexture( GL_TEXTURE_2D, whiteTexHandle ); 
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whiteTex );
}

void RenderTarget::renderBuffer()
{
	// Set the target for the fragment shader outputs
	m_drawBufs[1] = GL_COLOR_ATTACHMENT0;

	if(m_drawBufs[1] == GL_INVALID_ENUM)
		exit(EXIT_FAILURE);

	glDrawBuffers(1, m_drawBufs); // "1" is the size of DrawBuffers

	checkFramebuffer();
	// Unbind the framebuffer, and revert to default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int RenderTarget::checkFramebuffer()
{
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return -1;
}

const GLuint RenderTarget::getFBOHandle(){ return m_fboHandle; }
const GLuint RenderTarget::getTextureHandle(){ return m_renderTex2DHandle; }
