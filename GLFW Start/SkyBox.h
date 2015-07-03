#pragma once
#include "../dependencies/gl/include/glew.h"
#include "../dependencies/FreeImage/include/FreeImage.h"
#include "../dependencies/glm/glm.hpp"
#include "Image.h"
#include "GLSLShader.h"
#include "Quad.h"
#include "Logger.h"

class SkyBox
{
public:
	SkyBox(glm::vec3 center, std::string texImageFileName, float scale);
	~SkyBox();

	void initSkyBox();
	bool setTexImage(std::string texImageFileName);
	void setTexCoords(GLfloat* bottom, GLfloat* top, GLfloat* front, GLfloat* back, GLfloat* left, GLfloat* right);
	GLSLProgram& getShaderProgram(){ return m_shaderProg; }

	void drawSkyBox();

private:
	Quad** m_quads;
	float m_scale;
	glm::vec3 m_center;

	Bitmap m_texImage;

	GLuint m_skyboxTexID;
	GLfloat* m_texCoordsBottom;
	GLfloat* m_texCoordsTop;

	GLfloat* m_texCoordsFront;
	GLfloat* m_texCoordsBack;

	GLfloat* m_texCoordsLeft;
	GLfloat* m_texCoordsRight;
	GLSLProgram m_shaderProg;
};