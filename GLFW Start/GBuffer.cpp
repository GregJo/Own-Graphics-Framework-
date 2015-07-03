#pragma once
#include "GBuffer.h"
#include <cstdio>
#include <sstream>



bool GLErrorHandling(int line)
{
	GLenum error = glGetError();
    std::ostringstream o;

    switch(error) {
        case GL_NO_ERROR:
			printf("No errror occured! \n");
			o<<"No errror occured!"<<std::endl;
            return true;

        case GL_INVALID_ENUM:
			printf("OpenGL Error at line %d: Invalid enum! \n", line);
            o<<"OpenGL Error at line "<<line<<": Invalid enum!"<<std::endl;
			return false;

        case GL_INVALID_VALUE:
			printf("OpenGL Error at line %d: Invalid value! \n", line);
            o<<"OpenGL Error at line "<<line<<": Invalid value!"<<std::endl;
            return false;

        case GL_INVALID_OPERATION:
			printf("OpenGL Error at line %d: Invalid operation! \n", line);
            o<<"OpenGL Error at line "<<line<<": Invalid operation!"<<std::endl;
            return false;

        case GL_STACK_OVERFLOW:
			printf("OpenGL Error at line %d: Stack overflow! \n", line);
            o<<"OpenGL Error at line "<<line<<": Stack overflow!"<<std::endl;
            return false;

        case GL_STACK_UNDERFLOW:
			printf("OpenGL Error at line %d: Stack underflow! \n", line);
            o<<"OpenGL Error at line "<<line<<": Stack underflow!"<<std::endl;
            return false;

        case GL_OUT_OF_MEMORY:
			printf("OpenGL Error at line %d: Out Of memory! \n", line);
            o<<"OpenGL Error at line "<<line<<": Out Of memory!"<<std::endl;
            return false;

        case GL_TABLE_TOO_LARGE:
			printf("OpenGL Error at line %d: Table too large! \n", line);
            o<<"OpenGL Error at line "<<line<<": Table too large!"<<std::endl;
            return false;

        default:
			printf("OpenGL Error at line %d: Unknown error! \n", line);
            o<<"OpenGL Error at line "<<line<<": Unknown error!"<<std::endl;
            return false;
	}
}

GBuffer::GBuffer() //: m_fbo(0), m_depthTexture(0)
{
	//GLErrorHandling(63);
}

GBuffer::~GBuffer()
{ 
	delete[] m_textures; 
}

bool GBuffer::Init(unsigned int windowWidth, unsigned int windowHeight)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	//GLErrorHandling(74);

	int a = ARRAY_SIZE_IN_ELEMENTS(m_textures);
	glGenTextures(ARRAY_SIZE_IN_ELEMENTS(m_textures), m_textures);
	glGenTextures(1, &m_depthTexture);
	//GLErrorHandling(79);

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}

	//GLErrorHandling(90);

	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_STENCIL, 
                  GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

	glBindTexture(GL_TEXTURE_2D, m_finalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_finalTexture, 0);	

	//GLErrorHandling(96);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	int b = ARRAY_SIZE_IN_ELEMENTS(drawBuffers);
	glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(drawBuffers), drawBuffers);

	//GLErrorHandling(102);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status: 0x%x\n", status);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::StartFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::BindForGeomPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, 
                            GL_COLOR_ATTACHMENT1,
                            GL_COLOR_ATTACHMENT2 };

    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(drawBuffers), drawBuffers);
}

void GBuffer::BindForStencilPass()
{
    // must disable the draw buffers 
    glDrawBuffer(GL_NONE);
}

void GBuffer::BindForLightPass()
{
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
	//glClear(GL_COLOR_BUFFER_BIT);
	/*
    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_textures); i++) {
        glActiveTexture(GL_TEXTURE0 + i);	
        glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION + i]);
    }
	*/
}

void GBuffer::BindForFinalPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
}

void GBuffer::BindForWriting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void GBuffer::BindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}