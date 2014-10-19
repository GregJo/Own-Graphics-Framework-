#pragma once
#include "VAO.h"
#include "Logger.h"

VAO::~VAO()
{
	m_vbo->~VBO();
	glDeleteVertexArrays(1, &m_vertex_array_handle);
}

void VAO::InitVAO()
{
	glGenVertexArrays(1, &m_vertex_array_handle);
	glBindVertexArray(m_vertex_array_handle);
	if(GL_INVALID_OPERATION == m_vertex_array_handle)
		Logger::GetInstance().Log("Could not bind vertex array object.");

	m_vbo->InitVBO();

	glBindVertexArray(0);
}

void VAO::RenderVAO()
{
	glBindVertexArray(m_vertex_array_handle);

	if(m_vbo->getIndicesData() != nullptr)
	{
		int a = 1;
	}

	glDrawElements(m_vbo->getVertAlignment(), m_vbo->getIndicesCount(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}