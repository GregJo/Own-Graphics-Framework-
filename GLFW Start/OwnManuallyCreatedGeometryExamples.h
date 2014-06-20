#include "../dependencies/glm/glm.hpp"
#include "../dependencies/gl/include/glew.h"

//________________________________________________________________________________________________________________________________________
const GLfloat g_vertex_cube_buffer_data[] =	{
											-1.0f, -1.0f, -1.0f, 
											 1.0f,  1.0f, -1.0f, 
											-1.0f,  1.0f, -1.0f,  
											 1.0f, -1.0f, -1.0f,

											-1.0f, -1.0f, 1.0f, 
											 1.0f,  1.0f, 1.0f, 
											-1.0f,  1.0f, 1.0f,  
											 1.0f, -1.0f, 1.0f 
										};

static const GLfloat g_color_cube_buffer_data[] = 
{
    1.0f,    1.0f,    0.0f, 1,
    0.0f,    1.0f,    0.0f, 1,
    0.0f,    1.0f,    1.0f, 1,
    0.0f,    0.0f,    1.0f, 1,
    
    1.0f,    0.0f,    1.0f, 1,
    1.0f,    1.0f,    0.0f, 1,
    0.0f,    1.0f,    1.0f, 1,
    1.0f,    1.0f,    1.0f, 1,
};

//const GLfloat g_normals_cube_buffer_data[] =	{
//											0, 1, 0,
//											0, 1, 0,
//											0, 1, 0,
//										};

const GLubyte g_index_cube_buffer_data[] = { 
											0, 1, 2,
											0, 3, 1,

											3, 5, 1,
											3, 7, 5,

											7, 4, 6,
											7, 6, 5,

											4, 2, 6,
											4, 0, 2,

											2, 5, 6,
											2, 1, 5,

											0, 7, 4,
											0, 3, 7,
									  };

const unsigned int my_cube_v_count = 8;
const unsigned int my_cube_i_count = 36;


//________________________________________________________________________________________________________________________________________
const GLfloat g_vertex_triangle_buffer_data[] =	{
											 0.0f,   1.0f, 0.0f, 
											 1.0f,  -1.0f, 0.0f, 
											-1.0f,  -1.0f, 0.0f,  
										};

static const GLfloat g_color_triangle_buffer_data[] = 
{
    1.0f,    0.0f,    0.0f, 1,
    0.0f,    1.0f,    0.0f, 1,
    0.0f,    0.0f,    1.0f, 1
};

const GLfloat g_normals_triangle_buffer_data[] =	{
											0, 0, 1,
											0, 0, 1,
											0, 0, 1
										};

const GLubyte g_index_triangle_buffer_data[] = { 
											0, 1, 2
									  };

const unsigned int my_triangle_v_count = 3;
const unsigned int my_triangle_i_count = 3;