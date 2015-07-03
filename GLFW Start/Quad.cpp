#pragma once

#include "Quad.h"

//TODO: Create default texture render shader.
Quad::Quad(){}
Quad::Quad(glm::vec3 center_point, glm::vec3 position, float width, float height){}
// Width and height is meant for the viewport
Quad::Quad(glm::vec3 left_top, glm::vec3 left_bottom, glm::vec3 right_bottom, glm::vec3 right_top, glm::vec3 position, unsigned int width,  unsigned int height,  unsigned int mask) : m_position(position), m_texHandle(-1)
{
	//left_top += position;
	//left_bottom += position;
	//right_bottom += position;
	//right_top += position;

	m_width = width;
	m_height = height;

	m_vertex_buffer_data = new GLfloat[12];
	m_texture_coord_buffer_data = new GLfloat[8];
	m_normal_buffer_data = new GLfloat[12];
	m_color_buffer_data = nullptr;
	m_index_buffer_data = new GLuint[6];

	// Fill buffers with data.
	// Vertices
	m_vertex_buffer_data[0]  =  left_top.x;
	m_vertex_buffer_data[1]  =  left_top.y;
	m_vertex_buffer_data[2]  =  left_top.z;

	m_vertex_buffer_data[3]  =  left_bottom.x;
	m_vertex_buffer_data[4]  =  left_bottom.y;
	m_vertex_buffer_data[5]  =  left_bottom.z;

	m_vertex_buffer_data[6]  =  right_bottom.x;
	m_vertex_buffer_data[7]  =  right_bottom.y;
	m_vertex_buffer_data[8]  =  right_bottom.z;

	m_vertex_buffer_data[9]  =  right_top.x;
	m_vertex_buffer_data[10] =  right_top.y;
	m_vertex_buffer_data[11] =  right_top.z;

	// Normals
	m_normal_buffer_data[0]  =  0.0f;
	m_normal_buffer_data[1]  =  1.0f;
	m_normal_buffer_data[2]  =  0.0f;

	m_normal_buffer_data[3]  =  0.0f;
	m_normal_buffer_data[4]  =  1.0f;
	m_normal_buffer_data[5]  =  0.0f;

	m_normal_buffer_data[6]  =  0.0f;
	m_normal_buffer_data[7]  =  1.0f;
	m_normal_buffer_data[8]  =  0.0f;

	m_normal_buffer_data[9]  =  0.0f;
	m_normal_buffer_data[10] =  1.0f;
	m_normal_buffer_data[11] =  0.0f;

	// Texture coordinates
	m_texture_coord_buffer_data[6]  =  1.0f;
	m_texture_coord_buffer_data[7]  =  1.0f;

	m_texture_coord_buffer_data[4]  =  1.0f;
	m_texture_coord_buffer_data[5]  =  0.0f;

	m_texture_coord_buffer_data[2]  =  0.0f;
	m_texture_coord_buffer_data[3]  =  0.0f;

	m_texture_coord_buffer_data[0]  =  0.0f;
	m_texture_coord_buffer_data[1]  =  1.0f;

	//Index coordinates
	m_index_buffer_data[0]  =  0;
	m_index_buffer_data[1]  =  1;
	m_index_buffer_data[2]  =  2;

	m_index_buffer_data[3]  =  2;
	m_index_buffer_data[4]  =  3;
	m_index_buffer_data[5]  =  0;

	if(mask == 0)
	{
		m_render_target = nullptr;
	}
	else
	{
		m_render_target = new RenderTarget( width, height, mask );
		m_render_target->initAndBindRenderTargetTextures( width, height, mask );
		m_render_target->initDefaultTexture();
	}

	m_defaultShaderProgram = new GLSLProgram();
	m_defaultShaderProgram->initShaderProgram("Vert.glsl", "", "", "", "Frag.glsl");

	m_currentShaderProgram = m_defaultShaderProgram;
}

Quad::~Quad()
{
	delete[] m_vertex_buffer_data;
	delete[] m_texture_coord_buffer_data;
	delete[] m_normal_buffer_data;
	delete[] m_color_buffer_data;
	delete[] m_index_buffer_data;

	m_vao->~VAO();
	m_render_target->~RenderTarget();
	m_defaultShaderProgram->~GLSLProgram();
}

void Quad::setTexCoords(GLfloat texCoords[8])
{
	m_texture_coord_buffer_data[0]  =  texCoords[0];
	m_texture_coord_buffer_data[1]  =  texCoords[1];

	m_texture_coord_buffer_data[2]  =  texCoords[2];
	m_texture_coord_buffer_data[3]  =  texCoords[3];

	m_texture_coord_buffer_data[4]  =  texCoords[4];
	m_texture_coord_buffer_data[5]  =  texCoords[5];

	m_texture_coord_buffer_data[6]  =  texCoords[6];
	m_texture_coord_buffer_data[7]  =  texCoords[7];
}

void Quad::setGLSLProgram(GLSLProgram& shaderProgram)
{
	m_currentShaderProgram = &shaderProgram;
}

void Quad::initQuad()
{ 	
	// Based on tri's.
	CustomMesh* mesh = new CustomMesh( m_vertex_buffer_data,
							m_texture_coord_buffer_data,
							m_normal_buffer_data,
							m_color_buffer_data,
							m_index_buffer_data,
							4, 6,
							GL_TRIANGLES );

	VBO* vbo = new VBO(mesh);
	m_vao = new VAO(vbo, GL_TRIANGLES);

	m_vao->InitVAO();

	//initDefaultTexture();
}

void Quad::initDefaultTexture()
{
	// One pixel white texture 
	GLubyte whiteTex[] = { 255, 255, 255 }; 
	glActiveTexture( GL_TEXTURE0 ); 
	glGenTextures( 1, &m_texHandle ); 
	glBindTexture( GL_TEXTURE_2D, m_texHandle ); 
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whiteTex );
}

void Quad::bindDefaultTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_texHandle);
}

void Quad::enableRenderTarget()
{
	// Bind to texture's FBO 
	glBindFramebuffer(GL_FRAMEBUFFER, m_render_target->getFBOHandle());
	// Viewport for the texture 
	glViewport(0, 0, m_width, m_height);
}

// Render into the target texture inbetween those two functions later when in use.

void Quad::resetToDefaultTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//TODO: Add shader program as parameter
void Quad::draw()
{
	if (m_texHandle != (GLuint)-1)
	{
		glBindTexture(GL_TEXTURE_2D, m_texHandle);
	}
	//glBindTexture(GL_TEXTURE_2D, m_texHandle);

	if (m_render_target != nullptr)
		glBindTexture(GL_TEXTURE_2D, m_render_target->getTextureHandle());

	//m_render_target->renderBuffer();
	m_vao->RenderVAO();

	if (m_texHandle != (GLuint)-1)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}