#pragma once
#include "Mesh.h"
#include "GLSLShader.h"
#include "Logger.h"
#include <iterator>

ModelMesh::ModelMesh() : m_vertex_buffer_data(nullptr),
	m_texture_coord_buffer_data(nullptr),
	m_normal_buffer_data(nullptr), 
	m_color_buffer_data(nullptr),
	m_index_buffer_data(nullptr),
	m_vertices_count(0),
	m_indices_count(0),
	m_vert_alignment(GL_TRIANGLES)
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
		m_color_buffer_data = new GLfloat[4];

		m_color_buffer_data[0] = mesh->mColors[0]->r;
		m_color_buffer_data[1] = mesh->mColors[0]->g;
		m_color_buffer_data[2] = mesh->mColors[0]->b;
		m_color_buffer_data[3] = mesh->mColors[0]->a;

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

void ModelMesh::FindAdjacencies(const aiMesh* paiMesh)
{
	std::vector<GLuint> Indices;

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) 
	{
		// Current face we are working with.
        const aiFace& face = paiMesh->mFaces[i];
		// Necessity explained one comment below.
		Face* Unique = new Face();

        // If a position vector is duplicated in the VB we fetch the 
        // index of the first occurrence.
        for (unsigned int j = 0 ; j < 3 ; j++) 
		{ 
            unsigned int Index = face.mIndices[j];
            const aiVector3D& v = paiMesh->mVertices[Index];
			
			if (m_position_map.find(v) == m_position_map.end()) 
			{
                m_position_map[v] = Index;
            }
            else 
			{
                Index = m_position_map[v];
            }
			
			// Overwrite face Index to make it truely unique.
            Unique->addIndex(Index);
        }

		m_unique_faces_vector.push_back(*Unique);
	}
	// The new triangles with adjacency, which provide the new index buffer.
	std::vector< TriangleAdjacency > AdjTriIndices;
	// The associative map with all edges and alligning triangle faces.
	std::map< Edge, unsigned int, CompareEdge > EdgeAdjTriMap;
	// For all unique faces.
	for ( unsigned int i = 0 ; i < m_unique_faces_vector.size() ; ++i ) 
	{ 
		// Fill currently avaible information for the new adjacency triangle.
		TriangleAdjacency adjTri;
		adjTri.m_adjacency_idices[0] = m_unique_faces_vector.at(i).m_indices[0];
		adjTri.m_adjacency_idices[2] = m_unique_faces_vector.at(i).m_indices[1];
		adjTri.m_adjacency_idices[4] = m_unique_faces_vector.at(i).m_indices[2];

		// Out the new adjacency triangle in the vector.
		AdjTriIndices.push_back( adjTri );
		// 
		int index = AdjTriIndices.size()-1;

		// For all three edges of the current face.
		for( int j = 0 ; j < 3 ; ++j )  
		{
			// 
			Edge e(m_unique_faces_vector.at(i).m_indices[j], m_unique_faces_vector.at(i).m_indices[(j + 1) % 3]);
			//
			auto itEdge = EdgeAdjTriMap.find(e);
			if ( itEdge == EdgeAdjTriMap.end() )  
			{
				EdgeAdjTriMap[e] = index;
			}
			else  
			{
				// The current triangle.
				TriangleAdjacency & gtri1 = AdjTriIndices[index] ;
				// The current neighbor triangle alligning to the same current edge as the current trianle.
				TriangleAdjacency & gtri2 = AdjTriIndices[itEdge->second];

				// Find edge indices in gtri1.
				// Add the missing triangle index of gtri2 at the according place.
				// Find edge indices in gtri2.
				// Add the missing triangle index of gtri1 at the according place.
				for( int k = 0 ; k < 6 ; k += 2 )
				{
					if(gtri2.m_adjacency_idices[k] == e.m_a && gtri2.m_adjacency_idices[(k+2)%6] == e.m_b || gtri2.m_adjacency_idices[k] == e.m_b && gtri2.m_adjacency_idices[(k+2)%6] == e.m_a)
					{
						unsigned int oppositeFaceIndex = gtri2.m_adjacency_idices[(k+4)%6];
						for (int l = 0; l < 6; l+=2)
						{
							if(gtri1.m_adjacency_idices[l] == e.m_a && gtri1.m_adjacency_idices[(l+2)%6] == e.m_b || gtri1.m_adjacency_idices[l] == e.m_b && gtri1.m_adjacency_idices[(l+2)%6] == e.m_a)
							{
								gtri1.m_adjacency_idices[(l+1)%6] = oppositeFaceIndex;
								gtri2.m_adjacency_idices[(k+1)%6] = gtri1.m_adjacency_idices[(l+4)%6];
								break;
							}
						}
						break;
					}
				}
				EdgeAdjTriMap.erase( e );
			}
		}
	}

	if(!EdgeAdjTriMap.empty())
	{
		for (auto itEdge = EdgeAdjTriMap.begin(); itEdge != EdgeAdjTriMap.end(); ++itEdge)
		{
			Edge e = itEdge->first; 
			TriangleAdjacency & gtri = AdjTriIndices[itEdge->second];
			//EdgeAdjTriMap.erase(itEdge);
			for( int k = 0 ; k < 6 ; k += 2 )
			{
				if(gtri.m_adjacency_idices[k] == e.m_a && gtri.m_adjacency_idices[(k+2)%6] == e.m_b || gtri.m_adjacency_idices[k] == e.m_b && gtri.m_adjacency_idices[(k+2)%6] == e.m_a)
				{
					gtri.m_adjacency_idices[(k+1)%6] = gtri.m_adjacency_idices[(k+4)%6];
				}
			}
		}
	}
	EdgeAdjTriMap.clear();

	delete[] m_index_buffer_data;

	m_indices_count = AdjTriIndices.size()*6;
	m_index_buffer_data = new GLuint[m_indices_count];

	for (int i = 0; i < AdjTriIndices.size(); ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			m_index_buffer_data[i*6+j] = AdjTriIndices[i].m_adjacency_idices[j];
		}
	}
	std::vector<GLuint> v(m_index_buffer_data, m_index_buffer_data + AdjTriIndices.size() * 6);
	//std::vector<GLuint> v(std::begin(m_index_buffer_data), std::end(m_index_buffer_data));
	int a = 1;
	//m_index_buffer_data=&Indices[0];
}

void ModelMesh::initModelMesh()
{
	CustomMesh* mesh = new CustomMesh( m_vertex_buffer_data,
								m_texture_coord_buffer_data,
								m_normal_buffer_data,
								m_color_buffer_data,
								m_index_buffer_data,
								m_vertices_count, m_indices_count,
								m_vert_alignment );

	VBO* vbo = new VBO(mesh);
	m_vao = new VAO(vbo, m_vert_alignment);

	m_vao->InitVAO();
}

void ModelMesh::draw()
{
	m_vao->RenderVAO();
}

