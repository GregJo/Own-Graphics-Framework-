#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;

out vec2 UV;
out vec4 Position;

void main()
{
	UV = in_UV;
	gl_Position = vec4(in_Position,1.0f);
}