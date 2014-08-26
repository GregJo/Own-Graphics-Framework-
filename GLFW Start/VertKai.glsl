#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

uniform vec3 LightPosition; // Light position in eye coords.
uniform mat4 ViewMatrix;
//uniform mat4 NormalMatrix;
//uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

out vec4 Position;
out vec3 Normal;
out vec4 eyeLightPosition;

void main()
{
	//vec4 tnorm = normalize(NormalMatrix * vec4(in_Normal,1.0));
	Position = ViewMatrix * vec4(in_Position,1.0);
	Normal = in_Normal;
	eyeLightPosition = ViewMatrix * vec4(-LightPosition,1.0f);

	gl_Position = MVP * vec4(in_Position,1.0f);
}

//out vec2 UV;
//out vec3 eye_vert_pos;
//out vec3 light_pos;
//out vec3 normal;
//out vec3 vertex_to_light_vector;