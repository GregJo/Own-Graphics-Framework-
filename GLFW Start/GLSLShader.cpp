#pragma once

#include "../dependencies/gl/include/glew.h"
#include <fstream>
#include <vector>
#include <sys\stat.h>
#include "GLSLShader.h"
#include "Logger.h"

GLSLProgram::GLSLProgram(): 
	m_linked(false),
	m_handle(0),
	m_logString("")
{
	m_handle = glCreateProgram();
	if( 0 == m_handle )
	{
		Logger::GetInstance().Log("Shader program creation failed.");
		exit(1);
	}
};

GLSLProgram::~GLSLProgram(){}

//Helper functions
bool GLSLProgram::mf_fileExists(const std::string & fileName)
{
	struct stat buffer;   
	return (stat (fileName.c_str(), &buffer) == 0);
}

int GLSLProgram::mf_getUniformLocation(const char * name )
{
	return glGetUniformLocation(m_handle, name);
}

std::string GLSLProgram::mf_loadShaderAsString(const char * fileName)
{
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(fileName, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
	return VertexShaderCode;
}

bool GLSLProgram::mf_compileShaderFromFile(const char * fileName,
		GLSLShader::GLSLShaderType type)
{
	if(mf_fileExists(fileName))
	{
		return mf_compileShaderFromString(mf_loadShaderAsString(fileName), type);
	} else
	{
		m_logString = "";
		m_logString += "Shader log:\n%s";
		m_logString += "File does not exist!\n";
		Logger::GetInstance().Log(log().c_str());
		return false;
	}
}

bool GLSLProgram::mf_compileShaderFromString( const std::string & source,
		GLSLShader::GLSLShaderType type )
{
	GLuint shader;
	switch(type)
	{
	case GLSLShader::VERTEX: shader = glCreateShader( GL_VERTEX_SHADER );
		break;
	case GLSLShader::FRAGMENT: shader = glCreateShader( GL_FRAGMENT_SHADER );
		break;
	case GLSLShader::GEOMETRY: shader = glCreateShader( GL_GEOMETRY_SHADER );
		break;
	case GLSLShader::TESS_CONTROL: shader = glCreateShader( GL_TESS_CONTROL_SHADER );
		break;
	case GLSLShader::TESS_EVALUATION: shader = glCreateShader( GL_TESS_EVALUATION_SHADER );
		break;
	}
 
	const GLchar* shaderCode = source.c_str();
	const GLchar* codeArray[] = {shaderCode};
	glShaderSource( shader, 1, codeArray, NULL );

	glCompileShader( shader );

	GLint result;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
	if( GL_FALSE == result )
	{
		Logger::GetInstance().Log( "Vertex shader compilation failed!" );
		GLint logLen;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );
		if( logLen > 0 )
		{
			char * log_text = (char *)malloc(logLen);	
			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log_text);
			m_logString = "";
			m_logString += "Shader log:\n";
			m_logString.append(log_text);
			Logger::GetInstance().Log(log().c_str());
			free(log_text);
		}
		return false;
	}

	glAttachShader(m_handle, shader);

	return true;
}

void GLSLProgram::initShaderProgram(const std::string& vertexShaderSource, const std::string& geometryShaderSource, 
						const std::string& tessControlShaderSource, const std::string& tessEvalShaderSource, 
						const std::string& fragmentShaderSource)
{
	if(vertexShaderSource.length() > 0)
	{
		if( !mf_compileShaderFromFile(vertexShaderSource.c_str(),
			GLSLShader::VERTEX))
		{
			Logger::GetInstance().Log("Vertex shader failed to compile!");
			Logger::GetInstance().Log(log().c_str());
			exit(1);
		}
	}

	if(geometryShaderSource.length() > 0)
	{
		if( !mf_compileShaderFromFile(geometryShaderSource.c_str(),
			GLSLShader::GEOMETRY))
		{
			Logger::GetInstance().Log("Geometry shader failed to compile!");
			Logger::GetInstance().Log(log().c_str());
			exit(1);
		}
	}

	if(tessControlShaderSource.length() > 0)
	{
		if( !mf_compileShaderFromFile(tessControlShaderSource.c_str(),
			GLSLShader::TESS_CONTROL))
		{
			Logger::GetInstance().Log("Tesselation Control failed to compile!");
			Logger::GetInstance().Log(log().c_str());
			exit(1);
		}
	}

	if(tessEvalShaderSource.length() > 0)
	{
		if( !mf_compileShaderFromFile(tessEvalShaderSource.c_str(),
			GLSLShader::TESS_EVALUATION))
		{
			Logger::GetInstance().Log("Tesselation Evaluation shader failed to compile!");
			Logger::GetInstance().Log(log().c_str());
			exit(1);
		}
	}

	if(fragmentShaderSource.length() > 0)
	{
		if( !mf_compileShaderFromFile(fragmentShaderSource.c_str(),
			GLSLShader::FRAGMENT))
		{
			Logger::GetInstance().Log("Fragment shader failed to compile!");
			Logger::GetInstance().Log(log().c_str());
			exit(1);
		}
	}

	if( !link() )
	{
		Logger::GetInstance().Log("Shader program failed to link!", log().c_str());
		Logger::GetInstance().Log(log().c_str());
		exit(1);
	}
}

bool GLSLProgram::link()
{
	glLinkProgram( m_handle );

	GLint status;
	glGetProgramiv( m_handle, GL_LINK_STATUS, &status );
	if( GL_FALSE == status ) {
		Logger::GetInstance().Log("Failed to link shader program!");
		GLint logLen;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH,
			&logLen);
		if( logLen > 0 )
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(m_handle, logLen,
				&written, log);
			m_logString = "";
			m_logString += "Program log:\n";
			m_logString.append(log);
			Logger::GetInstance().Log(m_logString.c_str());
			free(log);
		}
		return false;
	}
	m_linked = true;
	return true;
}

void GLSLProgram::use()
{
	if(m_linked)
		glUseProgram(m_handle);

	GLint result = GL_FALSE;
    int infoLogLength;

	glGetProgramiv(m_handle, GL_LINK_STATUS, &result);
    glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
    char* ProgramErrorMessage = new char[infoLogLength+1];
    glGetProgramInfoLog(m_handle, infoLogLength, NULL, ProgramErrorMessage);

	if(infoLogLength > 0)
	{
		m_logString = "Shader Program linking status:";
		m_logString.append(ProgramErrorMessage);
		Logger::GetInstance().Log(log().c_str());
	}

	printActiveUniforms();
	printActiveAttribs();

	delete[] ProgramErrorMessage;
}

std::string GLSLProgram::log()
{
	return m_logString;
}

int GLSLProgram::getHandle()
{
	return m_handle;
}

bool GLSLProgram::isLinked()
{
	return m_linked;
}

void GLSLProgram::bindAttribLocation( GLuint location,
	const char * name)
{
	glBindAttribLocation(m_handle, mf_getUniformLocation(name), name);
}

void GLSLProgram::bindFragDataLocation( GLuint location,
	const char * name )
{
	glBindFragDataLocation(m_handle,mf_getUniformLocation(name),name);
}

void GLSLProgram::setUniform(const char *name,float x,float y,float z)
{
	glUniform3f(mf_getUniformLocation(name),x,y,z);
}
void GLSLProgram::setUniform(const char *name, const glm::vec3 & v)
{
	const GLfloat val[3] = {v.x,v.y,v.z};
	glUniform3fv(mf_getUniformLocation(name),1,val);
}
void GLSLProgram::setUniform(const char *name, const glm::vec4 & v)
{
	const GLfloat val[4] = {v.x,v.y,v.z,v.w};
	glUniform4fv(mf_getUniformLocation(name),1,val);
}
void GLSLProgram::setUniform(const char *name, const glm::mat4 & m)
{
	glUniformMatrix4fv(mf_getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}
void GLSLProgram::setUniform(const char *name, const glm::mat3 & m)
{
	glUniformMatrix3fv(mf_getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
}
void GLSLProgram::setUniform(const char *name, float val )
{
	glUniform1f(mf_getUniformLocation(name),val);
}
void GLSLProgram::setUniform(const char *name, int val )
{
	glUniform1i(mf_getUniformLocation(name),val);
}
void GLSLProgram::setUniform(const char *name, bool val )
{
	glUniform1i(mf_getUniformLocation(name),val);
}

void GLSLProgram::printActiveUniforms()
{
	GLint nUniforms, maxLen;
	glGetProgramiv( m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH,
		&maxLen);
	glGetProgramiv( m_handle, GL_ACTIVE_UNIFORMS,
		&nUniforms);
	GLchar * name = (GLchar *) malloc( maxLen );
		GLint size, location;
	GLsizei written;
	GLenum type;
	printf(" Location | Name\n");
	printf("------------------------------------------------\n");
	for( int i = 0; i < nUniforms; ++i ) {
		glGetActiveUniform( m_handle, i, maxLen, &written,
			&size, &type, name );
		location = glGetUniformLocation(m_handle, name);
		printf(" %-8d | %s\n", location, name);
	}
	free(name);
}

void GLSLProgram::printActiveAttribs()
{
	GLint maxLength, nAttribs;
	glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES,
		&nAttribs);
	glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
		&maxLength);

	GLchar * name = (GLchar *) malloc( maxLength );

	GLint written, size, location;
	GLenum type;
	printf(" Index | Name\n");
	printf("------------------------------------------------\n");
	for( int i = 0; i < nAttribs; i++ ) {
		glGetActiveAttrib( m_handle, i, maxLength, &written,
			&size, &type, name );
		location = glGetAttribLocation(m_handle, name);
		printf(" %-5d | %s\n", location, name);
	}
	free(name);
}