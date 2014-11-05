//1.
//2.
//3.
#include "../dependencies/gl/include/glew.h"

class RenderTarget
{
public:
	// Later on Implement: RenderTarget(Tex2d renderTex2D, bool depthBufferEnable)
	RenderTarget(unsigned int width, unsigned int height, bool depthBufferEnable) : m_fboHandle(-1), m_renderTex2DHandle(-1), m_depthBufHandle(-1), m_drawBufs(nullptr)
	{
		// Generate and bind the framebuffer 
		glGenFramebuffers(1, &m_fboHandle); 
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

		glGenTextures(1, &m_renderTex2DHandle); 
		glActiveTexture(GL_TEXTURE0);  // Use texture unit 0 glBindTexture(GL_TEXTURE_2D, renderTex);
		glBindTexture(GL_TEXTURE_2D, m_renderTex2DHandle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, m_renderTex2DHandle, 0);

		if(depthBufferEnable)
		{
			glGenRenderbuffers(1, &m_depthBufHandle); 
			glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufHandle); 
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		}

		// Bind the depth buffer to the FBO 
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufHandle); 
		// Set the target for the fragment shader outputs
		GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
		// How to do this better?
		m_drawBufs = drawBufs;
		delete[] drawBufs;
		glDrawBuffers(1, m_drawBufs); 
		// Unbind the framebuffer, and revert to default framebuffer 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const GLuint getFBOHandle(){}
	const GLuint getRenderTex2DHandle(){}
	const GLuint getDepthTex2DHandle(){}

	const GLuint getRenderTex2DData(){}

private:
	GLuint m_fboHandle;
	GLuint m_renderTex2DHandle;
	GLuint m_depthBufHandle;

	GLenum* m_drawBufs;
};