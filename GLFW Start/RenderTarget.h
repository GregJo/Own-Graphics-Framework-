#pragma once
#include "../dependencies/gl/include/glew.h"

enum RenderTargetTextureCoponents
{
	Color=2,Depth=4,Invalid=8
};

class RenderTarget
{
public:

	// Later on Implement: RenderTarget(Tex2d renderTex2D, bool depthBufferEnable)
	RenderTarget(unsigned int width, unsigned int height, unsigned int mask);

	inline bool IsValidMask( unsigned int mask);

	//TODO: Add error handling. (Adding default texture to know if something went wrong, etc.)
	void initAndBindRenderTargetTextures(unsigned int width, unsigned int height, unsigned int mask);

	~RenderTarget();

	void initDefaultTexture();

	void renderBuffer();

	int checkFramebuffer();

	const GLuint getFBOHandle();
	const GLuint getTextureHandle();

private:
	GLuint m_fboHandle;
	GLuint m_renderTex2DHandle;
	GLuint m_depthBufHandle;

	GLenum* m_drawBufs;
};