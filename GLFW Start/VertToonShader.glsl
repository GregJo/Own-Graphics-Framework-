#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

uniform mat4 ViewMatrix;
uniform mat4 NormalMatrix;
//uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

out vec2 UV;
out vec4 Position;
out vec3 Normal;
//out vec4 eyeLightPosition;

void main()
{
	//vec4 tnorm = normalize(NormalMatrix * vec4(in_Normal,1.0));
	UV = in_UV;
	Position = ViewMatrix * vec4(in_Position,1.0);
	Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	//eyeLightPosition = ViewMatrix * vec4(LightPosition,1.0f);

	gl_Position = MVP * vec4(in_Position,1.0f);
}