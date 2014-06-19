#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

out vec2 ex_UV;
out vec3 ex_vert_pos;
out vec3 ex_light_pos;
out vec3 ex_Normal;
out vec3 ex_vertex_to_light_vector;

uniform vec3 lightPos;// = vec4(0,100,200,1);
uniform mat4 MVP;
uniform mat4 TInvMVP;

void main()
{
	gl_Position = MVP * vec4(in_Position, 1);
	ex_Normal = in_Normal; //normalize(vec3((MVP * vec4(in_Normal, 0)).xyz));

	// Test
	ex_vert_pos = vec3(gl_Position)/gl_Position.w;
	ex_light_pos = lightPos;
	ex_UV = in_UV;
}