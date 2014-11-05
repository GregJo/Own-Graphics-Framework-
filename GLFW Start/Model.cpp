#pragma once
#include "Model.h"
#include "Logger.h"
#include <map>

Model::Model() : m_mesh_count(0), m_model_meshes(nullptr), m_shaderProgHandle(0)
{}

Model::~Model()
{
	//Delete ModelMeshes in m_model_meshes?
	delete[] m_model_meshes;
	delete[] m_textureIds;
}

bool Model::importLoadModel(const std::string pFile, unsigned int pFlags)
{
	// Extract the directory part from the file name
    std::string::size_type slashIndex = pFile.find_last_of("/");

    if (slashIndex == std::string::npos) {
        m_model_path_dir = ".";
    }
    else if (slashIndex == 0) {
        m_model_path_dir = "/";
    }
    else {
        m_model_path_dir = pFile.substr(0, slashIndex+1);
    }

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( pFile, pFlags );
	m_position = scene->mRootNode;

	if(!scene)  { Logger::GetInstance().Log(importer.GetErrorString()); return false; }

	//Access file content here.
	m_meshes = scene->mMeshes;

	aiMaterial* material;

	m_mesh_count = scene->mNumMeshes;

	m_model_meshes = new ModelMesh*[m_mesh_count];

	for(int i = 0; i < m_mesh_count; i++)
	{
		m_model_meshes[i] = new ModelMesh();
		m_model_meshes[i]->loadModelMesh(m_meshes[i]);
		if(m_vert_alignment == GL_TRIANGLES_ADJACENCY)
		{
			m_model_meshes[i]->FindAdjacencies(m_meshes[i]);
			m_model_meshes[i]->setVertAlignment(GL_TRIANGLES_ADJACENCY);
		}
		m_model_meshes[i]->initModelMesh();

		material = scene->mMaterials[m_meshes[0]->mMaterialIndex];
		//load Material
		m_model_meshes[i]->loadMaterial(material, m_shaderProgHandle);
	}

	loadTexture(scene);

	return true;
}

// TODO: Make it work with embedded textures.
bool Model::loadTexture(const aiScene* scene)
{
	std::map<std::string, GLuint> textureIdMap;

	for (unsigned int m=0; m<scene->mNumMaterials; ++m)
    {
        int texIndex = 0;
        aiString path;  // filename
 
        aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
        while (texFound == AI_SUCCESS) {
            // fill map with textures, OpenGL image ids set to 0
            textureIdMap[path.data] = 0; 
            // more textures?
            texIndex++;
            texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
        }
    }
 
    int numTextures = textureIdMap.size(); 
 
    /* create and fill array with GL texture ids */
	m_textureIds = new GLuint[numTextures];
	/* Texture name generation */ 
	glGenTextures(numTextures, m_textureIds);	

	/* get iterator */
    std::map<std::string, GLuint>::iterator itr = textureIdMap.begin();

	for (int i = 0; itr != textureIdMap.end(); ++i, ++itr)
    {
		std::string filename = (*itr).first.c_str();	// get filename
		unsigned found_double_backspace = (*itr).first.find_first_of("'\\' | '\'");
		if(found_double_backspace != std::string::npos)
		{
			std::string filename_tmp = filename.substr(0,found_double_backspace);
			filename_tmp.append("/");
			filename_tmp.append(filename.substr(found_double_backspace+1));
			filename = filename_tmp;
		}

		(*itr).second = m_textureIds[i];				// save texture id for filename in map

		Bitmap image;
		
		std::string total_path = m_model_path_dir;
		total_path.append(filename);

		bool success = image.loadFile(total_path);

		if (success) {
            /* Create and load textures to OpenGL */
			glBindTexture(GL_TEXTURE_2D, m_textureIds[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(),
				image.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE,
				image.getData()); 
        }
        else
		{
			Logger::GetInstance().Log("Couldn't load Image: %s\n", total_path.c_str());
			return false;
		}
    }

	return true;
}

void Model::setShaderProgram(GLuint shaderProgHandle)
{
	m_shaderProgHandle = shaderProgHandle;
}

void Model::loadMaterial(aiMaterial* material)
{

}

void Model::drawModel()
{
	for(int i = 0; i < m_mesh_count; i++)
	{
		//TODO: Set different samplers for different texture types(diffuse, specular, etc.) via glActiveTexture(GL_TEXTUREi)
		glBindTexture(GL_TEXTURE_2D, m_textureIds[i]);
		m_model_meshes[i]->bindMaterial();
		m_model_meshes[i]->drawModelMesh();
		//Set Samplers via SetUniform().
	}
}