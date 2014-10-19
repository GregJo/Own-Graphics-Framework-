#pragma once
#include "Mesh.h"
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

// TODO: Make sure everything is copied into the maps, to prevail memory leaks.
void ModelMesh::FindAdjacencies(const aiMesh* paiMesh)
{
	std::vector<GLuint> Indices;

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) 
	{
		// Current face we are working with.
        const aiFace& face = paiMesh->mFaces[i];
		// Necessity explained one comment below.
		Face Unique;

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
            Unique.addIndex(Index);
        }

		m_unique_faces_vector.push_back(Unique);

		Edge e1(Unique.m_indices[0], Unique.m_indices[1]);
        Edge e2(Unique.m_indices[1], Unique.m_indices[2]);
        Edge e3(Unique.m_indices[2], Unique.m_indices[0]);

		// Here is a 'Neighbours' data structure necessary.
		// Declare current face as neighbour to all edges it is consists of.
		
		bool isNotContained = (m_index_map.find(e1) == m_index_map.end());

		//Neighbors* n = nullptr;
		if (isNotContained) 
		{
			//Neighbors* n = new Neighbors();
			//m_index_map[e1] = *n;
			m_index_map[e1] = Neighbors();
        }
		isNotContained = (m_index_map.find(e2) == m_index_map.end());
		if (isNotContained) 
		{
			m_index_map[e2] = Neighbors();
        }
		isNotContained = (m_index_map.find(e3) == m_index_map.end());
		if (isNotContained) 
		{
			m_index_map[e3] = Neighbors();
        }
		
		m_index_map[e1].addNeighbor(i);
        m_index_map[e2].addNeighbor(i);
        m_index_map[e3].addNeighbor(i);
		
    }

	// Continue here.
	for (unsigned int i = 0 ; i < m_unique_faces_vector.size() ; i++) { 
        const Face& face = m_unique_faces_vector[i];
        for (unsigned int j = 0 ; j < 3 ; j++) { 
			// Rebuild the edges, which we advised Neighbours to (Edges of the current face)
            Edge e(face.m_indices[j], face.m_indices[(j + 1) % 3]);
            assert(m_index_map.find(e) != m_index_map.end());
			// Get the current face index to the current edge
            Neighbors n = m_index_map[e];
			// Get the opposite face index to the current edge
			if(n.m_neighbor_counter != 2)
			{
				Logger::GetInstance().Log("Not enough neighbors.");
			}
			assert(n.m_neighbor_counter == 2);

            unsigned int OtherTri = n.getOther(i);

			if(OtherTri == -1)
			{
				Logger::GetInstance().Log("OtherTri has an invalid value.");
			}
            assert(OtherTri != -1);

            const Face& OtherFace = m_unique_faces_vector[OtherTri];

			unsigned int OppositeIndex = -1;

			for(int k=0 ; k < 3 ; k++)
			{
				if(OtherFace.m_indices[k] != face.m_indices[j] && OtherFace.m_indices[k] != face.m_indices[(j + 1) % 3])
				{
					OppositeIndex = OtherFace.m_indices[k];
					break;
				}
			}

            Indices.push_back(face.m_indices[j]);
            //Indices.push_back(OppositeIndex); 
        }
    }
	int a = 1;
	m_indices_count = Indices.size();
	m_index_buffer_data=&Indices[0];
}

void ModelMesh::FindAdjacencies2(const aiMesh* paiMesh)
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
				TriangleAdjacency & gtri1 = AdjTriIndices[index] ;
				TriangleAdjacency & gtri2 = AdjTriIndices[itEdge->second];

				// Finde Edge Indices in gtri1
				// Füge nicht vorhanden Dreiecksindex zu gtri2 and die entsprechende Stelle
				// Finde Edge Indices in gtri2
				// Füge nicht vorhanden Dreiecksindex zu gtri1 and die entsprechende Stelle
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

void ModelMesh::drawModelMesh()
{
	m_vao->RenderVAO();
}

