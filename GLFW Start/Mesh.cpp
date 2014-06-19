#pragma once
#include "Mesh.h"
#include "Logger.h"

ModelMesh::ModelMesh() : m_vertex_buffer_data(nullptr),
	m_texture_coord_buffer_data(nullptr),
	m_normal_buffer_data(nullptr), 
	m_color_buffer_data(nullptr),
	m_index_buffer_data(nullptr),
	m_vertices_count(0),
	m_indices_count(0)
{}

ModelMesh::~ModelMesh()
{
	delete[] m_vertex_buffer_data;
	delete[] m_texture_coord_buffer_data;
	delete[] m_normal_buffer_data;
	delete[] m_color_buffer_data;
	delete[] m_index_buffer_data;
	m_material_uniform_buffer->~GLSLUniformBuffer();
	m_vao->~VAO();
}

// TODO: Channel select for texture coordinates and vertex colors
bool ModelMesh::loadModelMesh(aiMesh* mesh)
{
	m_vertices_count = mesh->mNumVertices;

	m_indices_count  = mesh->mNumFaces*3;//(*mesh)->mFaces->mNumIndices;

	if(mesh->HasPositions())
	{	
		m_vertex_buffer_data  = new GLfloat[m_vertices_count*3];
		
		for(int i=0; i<m_vertices_count; i++)
		{
			m_vertex_buffer_data[i*3]   = mesh->mVertices[i].x;
			m_vertex_buffer_data[i*3+1] = mesh->mVertices[i].y;
			m_vertex_buffer_data[i*3+2] = mesh->mVertices[i].z;
		}
	} 

	//TODO: Check for correctness.
	if(mesh->HasTextureCoords(0))
	{
		//if(mesh->mNumUVComponents[0] == 2)
		//{
		m_texture_coord_buffer_data  = new GLfloat[m_vertices_count*2];
			for(int i=0; i<m_vertices_count; i++)
			{
				m_texture_coord_buffer_data[i*2]   = mesh->mTextureCoords[0][i].x;
				m_texture_coord_buffer_data[i*2+1] = mesh->mTextureCoords[0][i].y;
			}
		//}
	}

	if(mesh->HasNormals())
	{
		m_normal_buffer_data = new GLfloat[m_vertices_count*3];
		for(int i=0; i<m_vertices_count; i++)
		{
			m_normal_buffer_data[i*3]   = mesh->mNormals[i].x;
			m_normal_buffer_data[i*3+1] = mesh->mNormals[i].y;
			m_normal_buffer_data[i*3+2] = mesh->mNormals[i].z;
		}
	}

	//TODO: Check for correctness.
	if(mesh->HasVertexColors(0))
	{
		m_color_buffer_data = new GLfloat[m_vertices_count*4];

		for(int i=0; i<m_vertices_count; i++)
		{
			m_color_buffer_data[i*4]   = mesh->mColors[i]->r;
			m_color_buffer_data[i*4+1] = mesh->mColors[i]->g;
			m_color_buffer_data[i*4+2] = mesh->mColors[i]->b;
			m_color_buffer_data[i*4+3] = mesh->mColors[i]->a;
		}
	}

	if(mesh->mNumFaces > 0)
	{
		m_index_buffer_data = new GLuint[m_indices_count];
		
		for(int i=0; i<mesh->mNumFaces; i++)
		{
			for(int j=0; j<mesh->mFaces[i].mNumIndices; j++)
			{
				m_index_buffer_data[i*3+j] = mesh->mFaces[i].mIndices[j];
			}
		}
	}

	mesh->mName.C_Str();

	if(m_vertex_buffer_data == nullptr)
	{
		Logger::GetInstance().Log("Model mesh has not been loaded: %s\n", mesh->mName.C_Str());
		return false;
	}
	else
	{
		return true;
	}
}

void ModelMesh::loadMaterial(aiMaterial* mat, GLuint shaderProgHandle)
{
 // Use this piece of code if one model mesh contains more then one texture (i.e. normal map, glossy map, ...).   
 //	  aiString texPath;   //contains filename of texture
 //   if(AI_SUCCESS == mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
 //           //bind texture
 //           //unsigned int texId = textureIdMap[texPath.data];
 //           //mesh.texIndex = texId;
 //           m_material.texCount = 1;
 //       }
 //   else
 //       m_material.texCount = 0;

	float c[4] = {1.0f, 1.0f, 0.0f, 1.0f};
    aiColor4D diffuse;
	if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
	{
		c[0]=diffuse.r;
		c[1]=diffuse.g;
		c[2]=diffuse.b;
		c[3]=diffuse.a;
	}
    memcpy(m_material.diffuse, c, sizeof(c));
	
	c[0]=0.0;
	c[1]=1.0;
	c[2]=1.0;
	c[3]=1.0;
	aiColor4D ambient;
    if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &ambient))
	{
		c[0]=ambient.r;
		c[1]=ambient.g;
		c[2]=ambient.b;
		c[3]=ambient.a;
	}
    memcpy(m_material.ambient, c, sizeof(c));

	c[0]=1.0;
	c[1]=0.5;
	c[2]=1.0;
	c[3]=1.0;
	aiColor4D specular;
    if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &specular))
	{
		c[0]=specular.r;
		c[1]=specular.g;
		c[2]=specular.b;
		c[3]=specular.a;
	}
    memcpy(m_material.specular, c, sizeof(c));

	c[0]=1;
	c[1]=0;
	c[2]=1;
	c[3]=1.0;
	aiColor4D emissive;
    if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &emissive))
	{
		c[0]=emissive.r;
		c[1]=emissive.g;
		c[2]=emissive.b;
		c[3]=emissive.a;
	}
    memcpy(m_material.emissive, c, sizeof(c));

	float shininess = 0.0;
    unsigned int max;
    aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max);
    m_material.shininess = shininess;

	m_material_uniform_buffer = new GLSLUniformBuffer("Material", shaderProgHandle);

	m_material_uniform_buffer->createUniformBuffer();
	m_material_uniform_buffer->copyFromSystemMemory((void*)(&m_material), sizeof(m_material));
}

void ModelMesh::initModelMesh()
{
	CustomMesh* mesh = new CustomMesh( m_vertex_buffer_data,
								m_texture_coord_buffer_data,
								m_normal_buffer_data,
								m_color_buffer_data,
								m_index_buffer_data,
								m_vertices_count, m_indices_count);

	VBO* vbo = new VBO(mesh);
	m_vao = new VAO(vbo);

	m_vao->InitVAO();
}

void ModelMesh::drawModelMesh()
{
	m_vao->RenderVAO();
}

