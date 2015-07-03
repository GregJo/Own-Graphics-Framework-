#pragma once
#include "DefferedRenderer.h"

DefferedRenderer::DefferedRenderer()
{
	m_Sampler = 0;
	glGenSamplers(1, &m_Sampler);

	m_GBuffer = new GBuffer();

	m_GeomPassGLSLProg =  new GLSLProgram();
	m_StencilPassGLSLProg =  new GLSLProgram();
	m_DirLightGLSLProg = new GLSLProgram();
	m_PointLightGLSLProg = new GLSLProgram();

	m_DirLightQuad = new Quad(glm::vec3(-1.0,1.0,0.0), glm::vec3(-1.0,-1.0,0.0), glm::vec3(1.0,-1.0,0.0), glm::vec3(1.0,1.0,0.0), glm::vec3(0.0), 0.0f,  0.0f, NULL);

	m_PointLight = new PointLight();
}

DefferedRenderer::~DefferedRenderer()
{
	m_DirLightGLSLProg->~GLSLProgram();
	m_PointLightGLSLProg->~GLSLProgram();
	m_GeomPassGLSLProg->~GLSLProgram();
	m_StencilPassGLSLProg->~GLSLProgram();
	
	m_GBuffer->~GBuffer();
	m_PointLight->~PointLight();
	m_DirLightQuad->~Quad();

	glDeleteSamplers(1, &m_Sampler);
}

void DefferedRenderer::InitDefferedRenderer(GraphicsWindow* graphicsWindow, unsigned int processFlagsOnModelImport = aiProcess_Triangulate|aiProcess_JoinIdenticalVertices|aiProcess_SortByPType)
{
	glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);  
	glSamplerParameteri(m_Sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	m_GBuffer->Init(graphicsWindow->getWindowWidth(), graphicsWindow->getWindowHeight());

	m_GeomPassGLSLProg->initShaderProgram("GeomPassVert.glsl", "", "", "", "GeomPassFrag.glsl");

	m_StencilPassGLSLProg->initShaderProgram("VertStencilPass.glsl", "", "", "", "FragStencilPass.glsl");

	m_DirLightGLSLProg->initShaderProgram("VertDirLight.glsl", "", "", "", "FragDirLight.glsl");

	m_DirLightQuad->initQuad();
	m_DirLightQuad->setGLSLProgram(*m_DirLightGLSLProg);
	
	m_PointLightGLSLProg->initShaderProgram("VertPointLight.glsl", "", "", "", "FragPointLight.glsl");
	
	m_PointLight->initLightSphere(processFlagsOnModelImport);
}

void DefferedRenderer::GeometryPass(Scene& scene, GraphicsWindow& graphicsWindow)
{ 
	m_GBuffer->StartFrame();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	m_GBuffer->BindForGeomPass();

    glDepthMask(GL_TRUE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	scene.updateCamera(graphicsWindow.getWindowHandle(), 0.0f);
	scene.drawScene();

    glDepthMask(GL_FALSE);

    glDisable(GL_DEPTH_TEST);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DefferedRenderer::DebugPass(GraphicsWindow& graphicsWindow)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_GBuffer->BindForReading();

	unsigned int windowWidth = graphicsWindow.getWindowWidth();
	unsigned int windowHeight = graphicsWindow.getWindowHeight();

	GLsizei HalfWidth = (GLsizei)(windowWidth / 2.0f);
    GLsizei HalfHeight = (GLsizei)(windowHeight / 2.0f);

    m_GBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
    glBlitFramebuffer(0, 0, windowWidth, windowHeight,
                    0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    m_GBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 
                    0, HalfHeight, HalfWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    m_GBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 
                    HalfWidth, HalfHeight, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    m_GBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 
                    HalfWidth, 0, windowWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void DefferedRenderer::BeginLightPasses()
{
    glEnable(GL_BLEND);
   	glBlendEquation(GL_FUNC_ADD);
   	glBlendFunc(GL_ONE, GL_ONE);

    m_GBuffer->BindForReading();
    glClear(GL_COLOR_BUFFER_BIT);
}

void DefferedRenderer::PointLightsPass( Camera& cam, glm::vec3 lightPositions[], glm::vec3 lightColors[], float lightScales[], GraphicsWindow& graphicsWindow, unsigned int numLights)
{	
	//Actual lightpass
	m_GBuffer->BindForLightPass();
	m_PointLight->getLightSphere()->getCurrentShaderProgram()->use();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	m_GBuffer->BindForReading();
    glClear(GL_COLOR_BUFFER_BIT);
	
	glBindSampler(0, m_Sampler);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GBuffer->getTextureHandles()[0]);
	//DRPointLightShaderProg->setUniform("gPositionMap", 0);

	glBindSampler(1, m_Sampler);
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D, m_GBuffer->getTextureHandles()[1]);
	//DRPointLightShaderProg->setUniform("gColorMap", 1);

	glBindSampler(2, m_Sampler);
	glActiveTexture(GL_TEXTURE0+2);
	glBindTexture(GL_TEXTURE_2D, m_GBuffer->getTextureHandles()[2]);
	//DRPointLightShaderProg->setUniform("gNormalMap", 2);

	// Warum?
	glActiveTexture(GL_TEXTURE31);

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	for (int i = 0; i < numLights; i++)
	{
		m_PointLight->setVPMatrixUniform( cam.getVPMatrix() );
		m_PointLight->setScreenSizeUniform( glm::vec2(graphicsWindow.getWindowWidth(),graphicsWindow.getWindowHeight()) );
		m_PointLight->setCamPositionUniform( cam.getCamPos() );
		m_PointLight->setColorUniform( lightColors[i] );
		m_PointLight->setAllPositionUniform( lightPositions[i] );
		m_PointLight->setAllScaleUniform( lightScales[i] );
		//m_PointLight->setDiffuseIntensityUniform( m_DiffuseIntensity);
		//m_PointLight->setSpecularIntensityUniform( m_SpecularIntensity);

		m_PointLight->getLightSphere()->draw();
	}

	glCullFace(GL_BACK);

	glDisable(GL_BLEND);
}

void DefferedRenderer::DirectionalLightPass(GraphicsWindow& graphicsWindow, glm::vec3 lightDir)// float intensity) 
{	
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	// Direct Light Pass
	m_DirLightQuad->getCurrentShaderProgram()->use();

	glBindSampler(0, m_Sampler);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GBuffer->getTextureHandles()[1]);
	//dirLightQuad.getCurrentShaderProgram()->setUniform("colorMap", 1);

	glBindSampler(1, m_Sampler);
	glActiveTexture(GL_TEXTURE0+1);
	glBindTexture(GL_TEXTURE_2D, m_GBuffer->getTextureHandles()[2]);
	//dirLightQuad.getCurrentShaderProgram()->setUniform("normalMap", 2);

	m_DirLightQuad->getCurrentShaderProgram()->setUniform("screenSize", glm::vec2(graphicsWindow.getWindowWidth(),graphicsWindow.getWindowHeight()));
	m_DirLightQuad->getCurrentShaderProgram()->setUniform("lightDir", lightDir);
	//m_DirLightQuad->getCurrentShaderProgram()->setUniform("intensity", intensity);

	glActiveTexture(GL_TEXTURE31);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	m_DirLightQuad->draw();

	glDisable(GL_BLEND);
}

void DefferedRenderer::LightPass(Camera& cam, glm::vec3 lightPositions[], glm::vec3 lightColors[], float lightScales[], glm::vec3 lightDir, GraphicsWindow& graphicsWindow, unsigned int numLights)
{
	glEnable(GL_STENCIL_TEST);
	StencilPass(cam, lightPositions, lightScales, numLights);
	PointLightsPass( cam, lightPositions, lightColors, lightScales, graphicsWindow, numLights);
	glDisable(GL_STENCIL_TEST);
	DirectionalLightPass( graphicsWindow, lightDir );
}

void DefferedRenderer::unbindInternal()
{ 		
	glBindSampler(GL_TEXTURE0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//void DefferedRenderer::StencilPass(glm::vec3 position, float scale, glm::mat4 VPMatrix)
//{
//	m_GBuffer->BindForStencilPass();
//	m_StencilPassGLSLProg->use();
//
//	glEnable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
//	glClear(GL_STENCIL_BUFFER_BIT);
//
//	glStencilFunc(GL_ALWAYS,0,0);
//
//	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
//    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
//
//	m_StencilPassGLSLProg->setUniform("worldMatrix", glm::translate(glm::mat4(1),position));
//	m_StencilPassGLSLProg->setUniform("scaleMatrix", glm::scale(glm::mat4(1),glm::vec3(scale)));
//	m_StencilPassGLSLProg->setUniform("VPMatrix", VPMatrix);
//
//	m_PointLight->getLightSphere()->draw();
//}

void DefferedRenderer::StencilPass(Camera& cam, glm::vec3 lightPositions[], float lightScales[], unsigned int numLights)
{
	// Stencil Test
	m_GBuffer->BindForStencilPass();
	m_StencilPassGLSLProg->use();

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS,0,0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);


	for (int i = 0; i < numLights; i++)
	{
		m_StencilPassGLSLProg->setUniform("worldMatrix", glm::translate(glm::mat4(1),lightPositions[i]));
		m_StencilPassGLSLProg->setUniform("scaleMatrix", glm::scale(glm::mat4(1),glm::vec3(lightScales[i])));
		m_StencilPassGLSLProg->setUniform("VPMatrix", cam.getVPMatrix());

		m_PointLight->getLightSphere()->draw();
	}

	glDisable(GL_DEPTH_TEST);
}

void DefferedRenderer::FinalPass(GraphicsWindow& graphicsWindow)
{
    m_GBuffer->BindForFinalPass();
	glBlitFramebuffer(0, 0, graphicsWindow.getWindowWidth(), graphicsWindow.getWindowHeight(), 
                      0, 0, graphicsWindow.getWindowWidth(), graphicsWindow.getWindowHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
}