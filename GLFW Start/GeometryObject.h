#pragma once
#include "GLSLShader.h"
#include "../dependencies/glm/glm.hpp"

class GeometryObject
{
public:
	//virtual GeometryObject* clone()=0;
	virtual ~GeometryObject()=0;
	// GeometryObject();
	virtual void setPosition(glm::vec3 position)=0;
	virtual glm::vec3 getPosition()=0;
	// draw
	virtual void draw()=0;
	// get shader effect
	virtual class GLSLProgram* getCurrentShaderProgram()=0;
};