#pragma once 
#include "../dependencies/FreeImage/include/FreeImage.h"
#include "Mesh.h"
#include "Image.h"
#include "../dependencies/glm/glm.hpp"

//TODO: Create logs.
//TODO: Create Material struct and a method that returns it.
//Errors to cover:

class Model
{
public:
	Model();
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

	//TODO: Convert aiNode m_position into glm::vec3 and return it. 
	glm::vec3 getModelPos()
	{	
		return glm::vec3(m_position->mTransformation.d1,m_position->mTransformation.d2,m_position->mTransformation.d3);
	}

	//! \brief Load mesh matrial.
	//! \param material Material of a mesh.
	void loadMaterial(aiMaterial* material);

	//TODO: Different samplers for different texture(diffuse) types. 
	void drawModel();

private:
	GLuint			m_shaderProgHandle;

	std::string		m_model_path_dir;
	aiMesh**		m_meshes;
	aiNode*			m_position; //Root of the aiScene of assimp.
	ModelMesh**		m_model_meshes;
	GLuint*			m_textureIds;

	unsigned int	m_mesh_count;
};