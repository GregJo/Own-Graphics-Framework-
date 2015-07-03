#pragma once 
#include "../dependencies/FreeImage/include/FreeImage.h"
#include "GLSLShader.h"
#include "Mesh.h"
#include "Image.h"
#include "GeometryObject.h"
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/glm/gtc/matrix_transform.hpp"

//TODO: Create logs.
//TODO: Create Material struct and a method that returns it.
//Errors to cover:
//TODO: Add a shader member variable and a default shader.

class Model : public GeometryObject
{
public:
	//! \param position Position of the model in worldspace.
	Model() { }
	Model(glm::vec3 position);
	~Model();

	//! \brief Load model as a scene.
	//! \param pFile Path of the model.
	//! \param pFlags Post process flags for assimp's model importer (flags for i.e. precalculated tangent space, flip UVs, etc.).
	//! \return true if loading succeeded.
	bool importLoadModel(const std::string pFile, unsigned int pFlags);

	//! \brief Load all Textures of the scene, often the model itself.
	//! \param scene Scene containing the model, often the model itself.
	//! \return true Return true if loading succeeded.
	bool loadTexture(const aiScene* scene);

	void setShaderProgram(GLuint shaderProgHandle);
	void setShaderProgram(GLSLProgram* shaderProgram);
	
	void useCurrentShaderProgram() { m_currentShaderProg->use(); }
	
	void setVertAlignment(GLenum vert_alignment) { m_vert_alignment = vert_alignment; }

	const GLenum getVertAlignment(GLenum vert_alignment) { return m_vert_alignment; }

	GLSLProgram* getCurrentShaderProgram() { return m_currentShaderProg; }

	void setPosition(glm::vec3 position)
	{	
		m_position = position;
	}

	void setScale(float scale)
	{	
		m_scale = scale;
	}

	glm::vec3 getPosition()
	{	
		return m_position;
	}

	glm::mat4 getWorldMatrix()
	{
		return glm::translate(glm::mat4(1), m_position);
	}

	float getScale()
	{	
		return m_scale;
	}

	glm::mat4 getScaleMatrix()
	{
		return glm::scale(glm::mat4(1),glm::vec3(m_scale,m_scale,m_scale)); 
	}

	//! \brief Load mesh matrial.
	//! \param material Material of a mesh.
	void loadMaterial(aiMaterial* material);

	//TODO: Different samplers for different texture(diffuse, glossy, etc.) types. 
	void draw();

private:
	GLSLProgram*	m_currentShaderProg;
	GLSLProgram*	m_defaultShaderProg;
	GLuint			m_shaderProgHandle;

	std::string		m_model_path_dir;
	aiMesh**		m_meshes;
	aiNode*			m_positionOriginal;				//Root of the aiScene of assimp.
	ModelMesh**		m_model_meshes;
	GLuint*			m_textureIds;

	float			m_scale;
	glm::vec3       m_position;

	GLenum			m_vert_alignment;

	unsigned int	m_mesh_count;
};