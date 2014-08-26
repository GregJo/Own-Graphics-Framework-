#pragma once
#include "../dependencies/gl/include/glew.h"
#include "../dependencies/glm/glm.hpp"
#include "../dependencies/assimp/include/assimp/Importer.hpp"
#include "../dependencies/assimp/include/assimp/scene.h"
#include "../dependencies/assimp/include/assimp/postprocess.h"
#include "VAO.h"
#include "GLSLUniformBuffer.h"
#include <map>
#include <vector>
#include <cassert>

//TODO: Implement
struct Edge
{
	Edge(unsigned int a, unsigned int b)
	{
		assert(a != b);
        
        if (a < b)
        {
            m_a = a;
            m_b = b;                   
        }
        else
        {
            m_a = b;
            m_b = a;
        }
	}
	~Edge(){}

	unsigned int m_a;
	unsigned int m_b;
};

struct Neighbors
{
public:
	Neighbors() : m_neighbor_indices(new unsigned int[2]), m_neighbor_counter(0) {}
	~Neighbors() {}
	
	void addNeighbor(unsigned int neighbor_index) 
	{
		assert(m_neighbor_counter < 2);
		m_neighbor_indices[m_neighbor_counter] = neighbor_index; 
		m_neighbor_counter++;
	}

	const unsigned int getOther(const unsigned int current_face_index)
	{
		if(current_face_index != m_neighbor_indices[0])
		{
			return m_neighbor_indices[0];
		}
		else
		{
			return m_neighbor_indices[1];
		} 
	}

private:
	unsigned int* m_neighbor_indices;
	unsigned int m_neighbor_counter;
};

struct CompareVector
{
	bool operator() (const aiVector3D& lhs,const aiVector3D& rhs)
	{
		if (lhs.x < rhs.x) {
            return true;
        }
        else if (lhs.x == rhs.x) {
            if (lhs.y < rhs.y) {
                return true;
            }
            else if (lhs.y == rhs.y) {
                if (lhs.z < rhs.z) {
                    return true;
                }
            }
        }
        
        return false;
	}
};

struct CompareEdge
{
	bool operator() (const Edge& lhs,const Edge& rhs)
	{
		if (lhs.m_a < lhs.m_a) {
            return true;
        }
        else if (lhs.m_a == lhs.m_a) {
            return (lhs.m_b < lhs.m_b);
        }        
        else {
            return false;
        }  
	}
};

class ModelMesh
{
public:
	struct Material
	{
		GLfloat diffuse[4];
		GLfloat ambient[4];
		GLfloat specular[4];
		GLfloat emissive[4];
		float shininess;
		//int texCount;
	};

	ModelMesh(); 
	~ModelMesh();

	//TODO: Channel count select for texture coordinates and vertex colors
	//! \brief Load mesh.
	//! \param mesh Model mesh from scene.
	//! \return true Return true if loading succeeded.
	bool loadModelMesh(aiMesh* mesh);

	//! \brief Load File to stream into.
	//! \param mat Matrial to convert to the Material struct and send it's data to the shader side uniform block. 
	//! \param shaderProgHandle Shader program containing the corresponding uniform block to the material.
	//! \return true Return true if loading succeeded.
	void loadMaterial(aiMaterial* mat, GLuint shaderProgHandle);

	void setVertAlignment(GLenum vert_alignment){ m_vert_alignment = vert_alignment; }

	//! \brief Fill and create VAO's
	void initModelMesh();

	//TODO: Implement
	void ModelMesh::FindAdjacencies(const aiMesh* paiMesh);
	void createTriangleAdjacencyBuffers();

	void bindMaterial()	{ m_material_uniform_buffer->bindUniformBuffer(); }

	void drawModelMesh();

private:

	GLfloat* m_vertex_buffer_data;
	GLfloat* m_texture_coord_buffer_data;
	GLfloat* m_normal_buffer_data;
	GLfloat* m_color_buffer_data;
	GLuint*  m_index_buffer_data;

	unsigned int m_vertices_count;
	unsigned int m_indices_count;

	std::vector<aiFace> m_unique_faces_map;
	std::map<aiVector3D, unsigned int, CompareVector> m_position_map;
	std::map<Edge, Neighbors, CompareEdge> m_index_map;

	GLenum m_vert_alignment;

	VAO* m_vao;

	Material m_material;

	GLSLUniformBuffer* m_material_uniform_buffer;
};