#pragma once
#include "GLSLUniformBuffer.h"
#include "Logger.h"

void GLSLUniformBuffer::createUniformBuffer()
{
	if(GL_INVALID_OPERATION == m_shaderProgHandle)
		Logger::GetInstance().Log("Shader program changed.");

	m_blockIndex = glGetUniformBlockIndex(m_shaderProgHandle, m_name.c_str());

	glGetActiveUniformBlockiv(m_shaderProgHandle, m_blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_blockSize);

	//if(GL_INVALID_INDEX == m_blockIndex)
		//Logger::GetInstance().Log("Invalid active uniformblock index.");
	
	//if(m_blockSize == 0)
		//Logger::GetInstance().Log("No active uniformblocks avaible.");
}

void GLSLUniformBuffer::copyFromSystemMemory(void* data, const unsigned int dataSizeInBytes)
{
	glGenBuffers( 1, &m_uniformBufferHandle );
	glBindBuffer( GL_UNIFORM_BUFFER, m_uniformBufferHandle );
	glBufferData( GL_UNIFORM_BUFFER, m_blockSize, data, GL_STATIC_DRAW );
}

void GLSLUniformBuffer::bindUniformBuffer()
{
	if(m_blockIndex == GL_INVALID_INDEX)
	{
		//Logger::GetInstance().Log("No valid active inactive block.");
		return;
	}

	if(m_uniformBufferHandle != GL_INVALID_VALUE)
		glBindBufferBase( GL_UNIFORM_BUFFER, m_blockIndex, m_uniformBufferHandle );
	else
		Logger::GetInstance().Log("Binding of the uniform buffer unsuccessful.");
}