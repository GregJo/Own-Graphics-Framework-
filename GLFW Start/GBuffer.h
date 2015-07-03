#pragma once
#include "../dependencies/gl/include/glew.h"
#include "GLSLShader.h"
#include "Quad.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

class GBuffer
{
public:

    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };

	GBuffer();

	~GBuffer();

    bool Init( unsigned int windowWidth, unsigned int windowHeight );

	void StartFrame();
    void BindForGeomPass();
    void BindForStencilPass();
    void BindForLightPass();
    void BindForFinalPass();

    void BindForWriting();
    void BindForReading();

	void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

	GLuint* getTextureHandles()
	{ return m_textures; }

	GLuint getGBufferFBOHandle()
	{ return m_fbo; }

private:
    GLuint m_fbo;
    GLuint m_textures[GBUFFER_NUM_TEXTURES];
    GLuint m_depthTexture;
	GLuint m_finalTexture;
};