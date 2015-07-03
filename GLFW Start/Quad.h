#pragma once
#include "../dependencies/glm/glm.hpp"
#include "Mesh.h"
#include "RenderTarget.h"
#include "GLSLShader.h"
#include "GeometryObject.h"
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"

//TODO: Create default texture render shader.

class Quad : public GeometryObject
{
public:
	Quad();
	Quad(glm::vec3 center_point, glm::vec3 position, float width, float height);
	// Width and height is meant for the viewport
	Quad(glm::vec3 left_top, glm::vec3 left_bottom, glm::vec3 right_bottom, glm::vec3 right_top, glm::vec3 position, unsigned int width,  unsigned int height, unsigned int mask);

	~Quad();

	void initQuad();

	void setTexCoords(GLfloat texCoords[8]);

	void setGLSLProgram(GLSLProgram& shaderProgram);

	void enableRenderTarget();

	void resetToDefaultTarget();

	void initDefaultTexture();

	void bindDefaultTexture();

	void setPosition(glm::vec3 position)
	{	
		m_position = position;
	}

	glm::vec3 getPosition() { return m_position; } 

	glm::mat4 getWorldMatrix()
	{
		return glm::translate(glm::mat4(1), m_position);
	}

	void draw();

	class GLSLProgram* getCurrentShaderProgram(){ return m_currentShaderProgram; }

private:

	glm::vec3 m_position;

	GLuint m_width;
	GLuint m_height;

	GLuint m_texHandle;

	GLfloat* m_vertex_buffer_data;
	GLfloat* m_texture_coord_buffer_data;
	GLfloat* m_normal_buffer_data;
	GLfloat* m_color_buffer_data;
	GLuint* m_index_buffer_data;

	VAO* m_vao;

	RenderTarget* m_render_target;

	GLSLProgram* m_defaultShaderProgram;
	GLSLProgram* m_currentShaderProgram;
};