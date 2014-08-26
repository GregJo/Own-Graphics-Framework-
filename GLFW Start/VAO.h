// Initialisierung und Verwendung des VAO.
#include "VBO.h"

class VAO
{
public:
	VAO(VBO* vbo, GLenum vert_alignment) : m_vertex_array_handle(0), m_vbo(vbo), m_vert_alignment(vert_alignment){}
	~VAO();

	void InitVAO();
	void RenderVAO();

private:
	GLuint m_vertex_array_handle;
	VBO* m_vbo; 
	GLenum m_vert_alignment;
};