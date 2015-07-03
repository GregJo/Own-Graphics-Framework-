#pragma once
#include "../dependencies/glm/glm.hpp"
#include "Model.h"
#include "Camera.h"


class PointLight
{
public:
	PointLight();
	//PointLight(float scale, float diffuseIntensity, float SpecularIntensity, glm::vec2 screenSize);
	~PointLight();

	void initLightSphere(unsigned int processFlagsOnModelImport);

	void setWorldMatrixUniform( glm::vec3 position)
	{ m_PointLightDRProgram->setUniform("worldMatrix", glm::translate(glm::mat4(1),position)); }
	void setScaleMatrixUniform( float scale)
	{ m_PointLightDRProgram->setUniform("scaleMatrix", glm::scale(glm::mat4(1),glm::vec3(scale))); }
	void setVPMatrixUniform( glm::mat4 VPMatrix )
	{ m_PointLightDRProgram->setUniform("VPMatrix", VPMatrix); }

	void setScreenSizeUniform( glm::vec2 screenSize)
	{ m_PointLightDRProgram->setUniform("screenSize", screenSize); }
	void setCamPositionUniform( glm::vec3 camPosition)
	{ m_PointLightDRProgram->setUniform("camPosition", camPosition); }
	void setPositionUniform( glm::vec3 position)
	{ m_PointLightDRProgram->setUniform("lightPosition", position); }
	void setColorUniform( glm::vec3 color)
	{ m_PointLightDRProgram->setUniform("lightColor", color); }
	void setScaleUniform( float scale)
	{ m_PointLightDRProgram->setUniform("lightSphereRadius", scale); }
	
	void setDiffuseIntensityUniform( float diffuseIntensity)
	{ m_PointLightDRProgram->setUniform("diffuseIntensity", diffuseIntensity); }
	void setSpecularIntensityUniform( float specularIntensity)
	{ m_PointLightDRProgram->setUniform("specularIntensity", specularIntensity); }

	void setAllPositionUniform( glm::vec3 position)
	{ 
		m_PointLightDRProgram->setUniform("lightPosition", position);
		m_PointLightDRProgram->setUniform("worldMatrix", glm::translate(glm::mat4(1),position));
	}
	void setAllScaleUniform( float scale)
	{ 
		m_PointLightDRProgram->setUniform("lightSphereRadius", scale);
		m_PointLightDRProgram->setUniform("scaleMatrix", glm::scale(glm::mat4(1),glm::vec3(scale)));
	}
	/*
	glm::vec3 getColor(){return m_Color;}
	glm::vec3 getPosition(){return m_Position;}

	float getScale(){return m_Scale;}
	float getDiffuseIntensity(){return m_DiffuseIntensity;}
	float getSpecularIntensity(){return m_SpecularIntensity;}
	*/
	GLSLProgram* getGLSLProgram(){ return m_PointLightDRProgram; }
	Model* getLightSphere(){ return m_LightSphere; }

private:
	Model* m_LightSphere;
	GLSLProgram* m_PointLightDRProgram;

	/*
	glm::vec3 m_Color;
	glm::vec3 m_Position;
	//glm::vec2 Attenuation;
	float m_Scale;
	float m_DiffuseIntensity;
	float m_SpecularIntensity;
	*/
};
