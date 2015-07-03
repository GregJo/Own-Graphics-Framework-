#pragma once
#include "PointLight.h"

PointLight::PointLight()
{
	/*
	m_Color = glm::vec3(0);
	m_Position = glm::vec3(0);

	m_Scale = 1.0f;
	m_DiffuseIntensity = 1.0f;
	m_SpecularIntensity = 1.0f;
	*/
	m_PointLightDRProgram = new GLSLProgram();
	m_PointLightDRProgram->initShaderProgram("VertPointLight.glsl","","","","FragPointLight.glsl");
}

PointLight::~PointLight()
{
	m_LightSphere->~Model();
	m_PointLightDRProgram->~GLSLProgram();
}

void PointLight::initLightSphere(unsigned int processFlagsOnModelImport)
{
	m_LightSphere = new Model(glm::vec3(1));
	//m_LightSphere->setPosition(m_Position);
	//m_LightSphere->setScale(m_Scale);
	m_LightSphere->setShaderProgram(m_PointLightDRProgram);
	m_LightSphere->importLoadModel("../Models/DRLightSphere/Sphere.obj", processFlagsOnModelImport);
}