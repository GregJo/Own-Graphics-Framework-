#pragma once
#include "CustomMesh.h"

CustomMesh::CustomMesh(const GLfloat* position, const GLfloat* texcoord, 
					   const GLfloat* normals, const GLfloat* color, 
			           const GLuint* indices, 
					   unsigned int vert_count, unsigned int indices_count,
					   GLenum vert_alignment) :
					   m_position(position),
					   m_texcoord(texcoord),
					   m_normals(normals),
					   m_color(color),
					   m_indices(indices),
					   m_position_set(false),
					   m_texcoord_set(false),
					   m_normals_set(false),
					   m_color_set(false),
					   m_indices_set(false),
					   m_vert_count(vert_count),
					   m_indices_count(indices_count),
					   m_vert_alignment(vert_alignment)
{
	m_vertDataSize = vert_count;
	m_vertElementSize = 0;
	if(position != nullptr)
	{ 
		m_position_set = true; 
		m_vertDataSize *= VERT_POSITION_SIZE; 
		m_vertElementSize += VERT_POSITION_SIZE; 
	} 
	if(texcoord != nullptr)
	{ 
		m_texcoord_set = true; 
		m_vertDataSize *= VERT_TEXCOORD_SIZE; 
		m_vertElementSize += VERT_TEXCOORD_SIZE; 
	}
	if(normals != nullptr)
	{ 
		m_normals_set = true; 
		m_vertDataSize *= VERT_NORMALS_SIZE; 
		m_vertElementSize += VERT_NORMALS_SIZE; 
	}
	if(color != nullptr)
	{ 
		m_color_set = true; 
		m_vertDataSize *= VERT_COLOR_SIZE; 
		m_vertElementSize += VERT_COLOR_SIZE; 
	}
	if(indices != nullptr)
	{ 
		m_indices_set = true; 
	}
}

CustomMesh::~CustomMesh()
{
	delete[] m_position;
	delete[] m_texcoord;
	delete[] m_normals;
	delete[] m_color;
	delete[] m_indices;
}

// Data is interleaved.
GLfloat* CustomMesh::getMeshVertDataAsArrayInterleaved()
{
	const unsigned int size = m_vertDataSize;
	int offset1 = 0;
	int offset2 = 0;
	int offset3 = 0;

	if(m_position_set)
	{
		offset1 += VERT_POSITION_SIZE;
	}

	if(m_texcoord_set)
	{
		offset2 += offset1 + VERT_TEXCOORD_SIZE;
	}

	if(m_normals_set)
	{
		offset3 += offset2 + VERT_NORMALS_SIZE;
	}

	// delete?
	GLfloat* mesh = new GLfloat[size];

	int i = 0;

	while(i < m_vert_count)
	{
		if(m_position_set)
		{
			mesh[i*m_vertElementSize] = m_position[i*VERT_POSITION_SIZE];
			mesh[i*m_vertElementSize+1] = m_position[i*VERT_POSITION_SIZE+1];
			mesh[i*m_vertElementSize+2] = m_position[i*VERT_POSITION_SIZE+2];
		}
		if(m_texcoord_set)
		{
			mesh[i*m_vertElementSize+offset1] = m_texcoord[i*VERT_TEXCOORD_SIZE];
			mesh[i*m_vertElementSize+offset1+1] = m_texcoord[i*VERT_TEXCOORD_SIZE+1];
		}
		if(m_normals_set)
		{
			mesh[i*m_vertElementSize+offset2] = m_normals[i*VERT_NORMALS_SIZE];
			mesh[i*m_vertElementSize+offset2+1] = m_normals[i*VERT_NORMALS_SIZE+1];
			mesh[i*m_vertElementSize+offset2+2] = m_normals[i*VERT_NORMALS_SIZE+2];
		}
		if(m_color_set)
		{
			mesh[i*m_vertElementSize+offset3] = m_color[i*VERT_COLOR_SIZE];
			mesh[i*m_vertElementSize+offset3+1] = m_color[i*VERT_COLOR_SIZE+1];
			mesh[i*m_vertElementSize+offset3+2] = m_color[i*VERT_COLOR_SIZE+2];
			mesh[i*m_vertElementSize+offset3+3] = m_color[i*VERT_COLOR_SIZE+3];
		}
		i++;
	}
	return mesh;
}

bool* CustomMesh::getMeshState()
{
	bool state[5] = {m_position_set,m_texcoord_set,m_normals_set,m_color_set,m_indices_set};
	return state;
}