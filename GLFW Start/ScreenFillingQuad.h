#pragma once
#include "../dependencies/glm/glm.hpp"
#include "Quad.h"

// TODO: Add further member variables, i.e. for clear color, etc.
class ScreenFillingQuad : public Quad, public GeometryObject // changes (i.e. over time) wanted, hence inheriting from GeometryObject
{
public:
	ScreenFillingQuad(unsigned int width, unsigned int height, glm::vec3 clearColor, unsigned int mask);
	
	~ScreenFillingQuad();

	void initScreenFillingQuad();

	// For a screenfilling quad no need for an extra framebuffer.
	void enableScreenFillingQuadRenderTarget();

	void disableScreenFillingQuadRenderTarget();

	void clearScreenFillingQuadRenderTarget();

	void setCurrentProgram(GLSLProgram shaderProgram)
	{
		Quad::setGLSLProgram(shaderProgram); // Warum funktioniert das?
	}

	GLSLProgram* getCurrentShaderProgram()
	{
		return Quad::getCurrentShaderProgram();
	}

	void setPosition(glm::vec3 position) { }

	glm::vec3 getPosition() { return glm::vec3(0); }

	void draw();

private:
	glm::vec3 m_clearColor;
};