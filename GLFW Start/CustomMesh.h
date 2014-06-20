#pragma once
#include "../dependencies/gl/include/glew.h"
#include <vector>

#define VERT_POSITION_SIZE	3 //sizeof(GLfloat[3])
#define VERT_TEXCOORD_SIZE	2 //sizeof(GLfloat[2])
#define VERT_NORMALS_SIZE	3 //sizeof(GLfloat[3])
#define VERT_COLOR_SIZE		4 //sizeof(GLfloat[4])
#define VERT_INDEX_SIZE		1 //sizeof(GLuint)

#define POSITION_MESH_STATE 0
#define TEXCOORD_MESH_STATE 1
#define NORMALS_MESH_STATE  2
#define COLOR_MESH_STATE	3
#define INDICES_MESH_STATE  4

struct Vertex
{
public:
private:
};

class CustomMesh
{
public:
	CustomMesh(const GLfloat* position, const GLfloat* texcoord, 
			   const GLfloat* normals, const GLfloat* color, 
			   const GLuint* indices, 
			   unsigned int vert_count, 
			   unsigned int indices_count);
	
	~CustomMesh();

	//! \brief Get all possible vertx data as an interleaved array, except index data.
	//! \returns Interleved vertex data array.
	GLfloat* getMeshVertDataAsArrayInterleaved();
	//! \brief Get mesh indices as an array.
	//! \returns Index data as array.
	const GLuint* getMeshIndexDataAsArray() { return m_indices; }
	//! \brief Get a boolean array indicating, which data has been loaded for the VBO initialization.
	//! \returns Boolean array of indicating, which data of the has been loaded.
	bool* getMeshState();

	const unsigned int getVertDataSize()	{ return m_vertDataSize; }
	const unsigned int getVertElementSize() { return m_vertElementSize; }
	const unsigned int getVertCount()		{ return m_vert_count; }
	const unsigned int getIndicesCount()	{ return m_indices_count; }

private:
	const GLfloat* m_position;
	const GLfloat* m_texcoord;
	const GLfloat* m_normals;
	const GLfloat* m_color;
	const GLuint* m_indices;

	bool m_position_set;
	bool m_color_set;
	bool m_texcoord_set;
	bool m_normals_set;
	bool m_indices_set;

	const unsigned int m_vert_count;
	const unsigned int m_indices_count;
	unsigned int m_vertDataSize;
	unsigned int m_vertElementSize;
};