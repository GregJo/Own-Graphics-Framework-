#pragma once
#include <string>
#include "..\dependencies\gl\include\glew.h"
#include "..\dependencies\glm\glm.hpp"

namespace GLSLShader {
	enum GLSLShaderType {
		VERTEX, FRAGMENT, GEOMETRY, TESS_CONTROL, TESS_EVALUATION
	};
};

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();
	
	//! \brief Load and compile the shaders to be used. Also link the shader program. 
	void initShaderProgram(const std::string& vertexShaderSource, const std::string& geometryShaderSource, 
							const std::string& tessControlShaderSource, const std::string& tessEvalShaderSource, 
							const std::string& fragmentShaderSource);
	
	//! \brief Link the shader program. 
	//! \returns true if linked successfully. 
	bool link();

	//! \brief Use the shader program. 
	void use();

	int getHandle();
	bool isLinked();
	
	//! \brief Bind uniform attribute.
	void bindAttribLocation( GLuint location,
		const char * name);
	//! \brief Bind the user-defined varying out variable 'name' to fragment shader color number 'colorNumber' for program 'program'.
	void bindFragDataLocation( GLuint location,
		const char * name );

	void setUniform(const char* name,float x,float y,
		float z);
	void setUniform(const char* name, const glm::vec3& v);
	void setUniform(const char* name, const glm::vec4& v);
	void setUniform(const char* name, const glm::mat4& m);
	void setUniform(const char* name, const glm::mat3& m);
	void setUniform(const char* name, float val );
	void setUniform(const char* name, int val );
	void setUniform(const char* name, bool val );

	//! \brief Print active uniforms for debug purpouses.
	void printActiveUniforms();
	//! \brief Print active attributes for debug purpouses.
	void printActiveAttribs();

private:
	int m_handle;
	bool m_linked;
	std::string m_logString;

	std::string log();
	std::string mf_loadShaderAsString(const char * fileName);
	int mf_getUniformLocation(const char * name );
	bool mf_fileExists( const std::string & fileName );
	bool mf_compileShaderFromString( const std::string & source,
		GLSLShader::GLSLShaderType type );
	bool mf_compileShaderFromFile( const char * fileName,
		GLSLShader::GLSLShaderType type );
};