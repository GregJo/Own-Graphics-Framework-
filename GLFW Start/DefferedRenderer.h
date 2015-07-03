#pragma once
#include "../dependencies/gl/include/glew.h"
#include "GLSLShader.h"
#include "../dependencies/glm/glm.hpp"
#include "Scene.h"
#include "GBuffer.h"
#include "PointLight.h"

class DefferedRenderer
{
public:
	DefferedRenderer();

	~DefferedRenderer();

	void InitDefferedRenderer(GraphicsWindow* graphicsWindow, unsigned int processFlagsOnModelImport);

	void GeometryPass(Scene& scene, GraphicsWindow& graphicsWindow);

	void DebugPass(GraphicsWindow& graphicsWindow);

	//void StencilPass(glm::vec3 position, float scale, glm::mat4 VPMatrix);

	void DefferedRenderer::StencilPass(Camera& cam, glm::vec3 lightPositions[], float lightScales[], unsigned int numLights);

	void BeginLightPasses();

	void PointLightsPass(Camera& cam, glm::vec3 lightPositions[], glm::vec3 lightColors[], float lightScales[], GraphicsWindow& graphicsWindow, unsigned int numLights);

	void DirectionalLightPass( GraphicsWindow& graphicsWindow, glm::vec3 lightDir);

	void LightPass(Camera& cam, glm::vec3 lightPositions[], glm::vec3 lightColors[], float lightScales[], glm::vec3 lightDir, GraphicsWindow& graphicsWindow, unsigned int numLights);

	void FinalPass(GraphicsWindow& graphicsWindow);

	void unbindInternal();

private:
	GLSLProgram* m_StencilPassGLSLProg;
	GLSLProgram* m_GeomPassGLSLProg;
	GLSLProgram* m_DirLightGLSLProg;
	GLSLProgram* m_PointLightGLSLProg;

	GLuint m_Sampler;

	GBuffer* m_GBuffer;
	PointLight* m_PointLight;
	Quad* m_DirLightQuad;
	glm::vec3 m_DirLight;
};