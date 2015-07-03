#pragma once
#include "Scene.h"

Scene::Scene(Camera* camera, SkyBox* skyBox, glm::vec3 ambientLight, glm::vec3 worldPosOrigin) : m_camera(camera), m_skyBox(skyBox), m_worldPosOrigin(worldPosOrigin)
{

}

Scene::~Scene()
{
	m_camera->~Camera();
	m_skyBox->~SkyBox();
	for (int i = 0; i < m_sceneModelList.size(); i++)
	{
		m_sceneModelList[i]->~GeometryObject(); // Implpement GeometryObject destructor or maybe overwrite??
	}

}

void Scene::addModel(GeometryObject* model)
{
	model->setPosition(m_worldPosOrigin);
	for (int i = 0; i < m_sceneModelList.size(); i++)
	{
		if(m_sceneModelList[i]==model)
		{ return; }
	}
	m_sceneModelList.push_back(model);
}

void Scene::sortByGLSLProgram()
{
	for (int i = 0; i < m_sceneModelList.size(); i++)
	{
		GeometryObject* model = m_sceneModelList[i];
		if(m_GLSLProgram_Model.count(model->getCurrentShaderProgram()) > 0)
		{
			std::map<GLSLProgram*,std::vector<GeometryObject*>>::iterator itr = m_GLSLProgram_Model.find(model->getCurrentShaderProgram());
			//if(std::find(itr->second.begin(), itr->second.end(), model) != itr->second.end())
			//{
			itr->second.push_back(model);
			//}
		}
		else
		{
			std::vector<GeometryObject*> vec;
			vec.push_back(model);
			m_GLSLProgram_Model.insert(std::pair<GLSLProgram*,std::vector<GeometryObject*>>(model->getCurrentShaderProgram(),vec));
		}
	}
}

void Scene::updateGLSLProgramUniforms(GLSLProgram* shader)
{
	shader->setUniform("VPMatrix",m_camera->getVPMatrix());
	//shader->setUniform("VPMatrix",m_camera->getVPMatrix()*glm::translate(glm::mat4x4(1.0), m_camera->getCamPos()));
	//shader->setUniform("projectionMatrix", m_camera->getPMatrix());
	//shader->setUniform("worldMatrix", m_camera->getVMatrix());
	shader->setUniform("viewMatrix", m_camera->getVMatrix());
	shader->setUniform("normalMatrix", m_camera->getTranspInvMVMatrix());
	shader->setUniform("lightPosition", m_camera->getCamPos());
	shader->setUniform("camPosition", m_camera->getCamPos());
}

void Scene::updateCamera(GLFWwindow* window, float time)
{
	//if (true)
	//{
	//	//m_camera->camControll(window_n1->getWindowHandle());
	//	//m_camera->update();
	//}
	//else if(false)
	//{
	//	//m_camera->update(time);
	//}
	m_camera->camControll(window);
	m_camera->update();
	//m_camera->update(time);
}

void Scene::drawScene()
{
	//m_skyBox->getShaderProgram().use();
	//updateGLSLProgramUniforms(&m_skyBox->getShaderProgram());
	//m_skyBox->drawSkyBox();
	
	for (std::map<GLSLProgram*,std::vector<GeometryObject*>>::iterator itr = m_GLSLProgram_Model.begin(); itr != m_GLSLProgram_Model.end(); itr++)
	{
		itr->first->use();
		updateGLSLProgramUniforms(itr->first);
		
		for (int i = 0; i < itr->second.size(); i++)
		{
			itr->first->setUniform("worldMatrix", glm::translate(glm::mat4(1), itr->second[i]->getPosition()));
			itr->first->setUniform("normalMatrix", glm::translate(glm::mat4(1), itr->second[i]->getPosition()));
			//itr->first->setUniform("normalMatrix", glm::transpose(glm::inverse(glm::translate(glm::mat4(1), itr->second[i]->getPosition()))));
			itr->second[i]->draw();
		}
	}
	glUseProgram(0);
}