#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

uniform mat4 VPMatrix;

out vec2 UV;
//out vec4 eyeLightPosition;

void main()
{
	UV = in_UV;

	gl_Position = VPMatrix * vec4(in_Position, 1.0f);
}