#pragma once
#include <string>
#include <vector>
#include <gl\glew.h>

//Errors to cover:
class GLSLUniformBuffer
{
public:
	GLSLUniformBuffer(const std::string name, GLuint shaderProgHandle): m_name(name),
																		m_uniformBufferHandle(0),
																		m_shaderProgHandle(shaderProgHandle),
																		m_blockIndex(0),
																		m_blockSize(0)
	{}
	~GLSLUniformBuffer(){}

	//TODO: Implement
	void setUBOName(const std::string name);
	void setUBOShaderProg(GLuint shaderProgHandle);

	void createUniformBuffer();
	//Maybe an extra Parameter for usage (GL_STATIC_DRAW, ...), or another method?
	void copyFromSystemMemory(void* data, const unsigned int dataSizeInBytes);

	void bindUniformBuffer();

private:
	const std::string m_name;
	GLuint m_uniformBufferHandle;
	GLuint m_shaderProgHandle;
	GLuint m_blockIndex;
	GLint  m_blockSize;
};