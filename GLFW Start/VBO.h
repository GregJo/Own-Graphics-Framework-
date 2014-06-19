#pragma once
#include <GL/glew.h>
#include "CustomMesh.h"

class VBO
{
public:
	VBO(CustomMesh* mesh) : m_mesh(mesh), m_mesh_BO_handle(0), m_created_mesh_BO_handle(false), m_created_mesh_index_BO_handle(false) {}
	~VBO();

	void InitVBO();

	GLuint getIndicesCount(){ return m_mesh->getIndicesCount(); }
	const GLuint* getIndicesData(){ return m_mesh->getMeshIndexDataAsArray(); }

private:
	CustomMesh* m_mesh;
	GLuint m_mesh_BO_handle;
	GLuint m_mesh_index_BO_handle;
	bool m_created_mesh_BO_handle;
	bool m_created_mesh_index_BO_handle;
};