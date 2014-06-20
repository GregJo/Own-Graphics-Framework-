#pragma once
#include "../dependencies/gl/include/glew.h"
#include "../dependencies/assimp/include/assimp/Importer.hpp"
#include "../dependencies/assimp/include/assimp/scene.h"
#include "../dependencies/assimp/include/assimp/postprocess.h"
#include "VAO.h"
#include "GLSLUniformBuffer.h"

class ModelMesh
{
public:
	struct Material
	{
		GLfloat diffuse[4];
		GLfloat ambient[4];
		GLfloat specular[4];
		GLfloat emissive[4];
		float shininess;
		//int texCount;
	};

	ModelMesh();
	~ModelMesh();

	//TODO: Channel select for texture coordinates and vertex colors
	//! \brief Load mesh.
	//! \param mesh Model mesh from scene.
	//! \return true Return true if loading succeeded.
	bool loadModelMesh(aiMesh* mesh);

	//! \brief Load File to stream into.
	//! \param mat Matrial to convert to the Material struct and send it's data to the shader side uniform block. 
	//! \param shaderProgHandle Shader program containing the corresponding uniform block to the material.
	//! \return true Return true if loading succeeded.
	void loadMaterial(aiMaterial* mat, GLuint shaderProgHandle);

	//! \brief Fill and create VAO's
	void initModelMesh();

	void bindMaterial()	{ m_material_uniform_buffer->bindUniformBuffer(); }

	void drawModelMesh();

private:
	GLfloat* m_vertex_buffer_data;
	GLfloat* m_texture_coord_buffer_data;
	GLfloat* m_normal_buffer_data;
	GLfloat* m_color_buffer_data;
	GLuint*  m_index_buffer_data;

	unsigned int m_vertices_count;
	unsigned int m_indices_count;

	VAO* m_vao;

	Material m_material;

	GLSLUniformBuffer* m_material_uniform_buffer;
};