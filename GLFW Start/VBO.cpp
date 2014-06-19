#pragma once
#include "VBO.h"
#include <stddef.h>
#include "Logger.h"

VBO::~VBO()
{
	m_mesh->~CustomMesh();
	if(m_created_mesh_BO_handle)
		glDeleteBuffers(1, &m_mesh_BO_handle);
	if(m_created_mesh_index_BO_handle)
		glDeleteBuffers(1, &m_mesh_index_BO_handle);
}

void VBO::InitVBO()
{
	unsigned int stride = m_mesh->getVertElementSize()*sizeof(GLfloat);
	unsigned int offset = 0;
	GLfloat* data = m_mesh->getMeshVertDataAsArrayInterleaved();

	glGenBuffers(1, &m_mesh_BO_handle);
	if(m_mesh_BO_handle != GL_INVALID_VALUE)
	{
		m_created_mesh_BO_handle = true;
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh_BO_handle);
	}
	else
		Logger::GetInstance().Log("Vertex buffer object handle was not created.");

	//All data of a mesh into an array
	int i = 0;

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_mesh->getVertDataSize(), data, GL_STATIC_DRAW);

	//Fill BO
	if(m_mesh->getMeshState()[POSITION_MESH_STATE])
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, VERT_POSITION_SIZE, GL_FLOAT, GL_FALSE, stride, 0);
		i++;
		offset += VERT_POSITION_SIZE;
	}
	if(m_mesh->getMeshState()[TEXCOORD_MESH_STATE])
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, VERT_TEXCOORD_SIZE, GL_FLOAT, GL_FALSE, stride, (void*)(offset*sizeof(GLfloat)));
		i++;
		offset += VERT_TEXCOORD_SIZE;
	}
	if(m_mesh->getMeshState()[NORMALS_MESH_STATE])
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, VERT_NORMALS_SIZE, GL_FLOAT, GL_FALSE, stride, (void*)(offset*sizeof(GLfloat)));
		i++;
		offset += VERT_NORMALS_SIZE;
	}
	if(m_mesh->getMeshState()[COLOR_MESH_STATE])
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, VERT_COLOR_SIZE, GL_FLOAT, GL_FALSE, stride, (void*)(offset*sizeof(GLfloat)));
	}

	if(m_mesh->getMeshState()[INDICES_MESH_STATE])
	{
		GLuint indices_buffer;
		glGenBuffers(1, &indices_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh->getIndicesCount() * sizeof(GLuint), m_mesh->getMeshIndexDataAsArray(), GL_STATIC_DRAW);

		m_mesh_index_BO_handle = indices_buffer;
		m_mesh_index_BO_handle = true;
	}
}