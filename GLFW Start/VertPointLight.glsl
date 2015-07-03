#version 420

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;

uniform mat4 worldMatrix = mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);

uniform mat4 scaleMatrix = mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);

uniform mat4 VPMatrix;

out vec2 UV;

void main()
{
	UV = in_UV;

	gl_Position = VPMatrix * worldMatrix * scaleMatrix * vec4(in_Position,1.0f);
}