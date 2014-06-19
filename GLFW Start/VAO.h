// Initialisierung und Verwendung des VAO.
#include "VBO.h"

class VAO
{
public:
	VAO(VBO* vbo) : m_vertex_array_handle(0), m_vbo(vbo){}
	~VAO();

	void InitVAO();
	void RenderVAO();

private:
	GLuint m_vertex_array_handle;
	VBO* m_vbo; 
};