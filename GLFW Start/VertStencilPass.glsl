#version 400

layout(location = 0) in vec3 in_Position;

uniform mat4 worldMatrix = mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);

uniform mat4 scaleMatrix = mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);

uniform mat4 VPMatrix;

void main()
{
	gl_Position = VPMatrix * worldMatrix * scaleMatrix * vec4(in_Position,1.0f);
}