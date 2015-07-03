#pragma once
#include <stdio.h>
#include <map>
#include "GeometryObject.h"
#include "../dependencies/glm/glm.hpp"
#include "Camera.h"
#include "SkyBox.h"
#include "GraphicsWindow.h"

class Scene
{
public:
	Scene(Camera* camera, SkyBox* skyBox, glm::vec3 ambientLight, glm::vec3 worldPosOrigin);
	~Scene();

	void addModel(GeometryObject* model);

	// Functions as name suggests, implement if more performance is needed.
	void sortByGLSLProgram();
	
	// Update shader Uniforms and call update() of objects.
	void updateGLSLProgramUniforms(GLSLProgram* shader);
	void updateCamera(GLFWwindow* window, float time);
	void drawScene();

	Camera* getCamera() 
	{ return m_camera; }

private:
	glm::vec3 m_ambientLight;
	glm::vec3 m_worldPosOrigin;
	std::vector<GeometryObject*> m_sceneModelList;
	std::map<GLSLProgram*,std::vector<GeometryObject*>> m_GLSLProgram_Model;
	Camera* m_camera;
	SkyBox* m_skyBox;
};