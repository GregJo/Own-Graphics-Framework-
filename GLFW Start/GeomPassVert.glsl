#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

uniform mat4 VPMatrix;
uniform mat4 worldMatrix = mat4(1,0,0,0,
								0,1,0,0,
								0,0,1,0,
								0,0,0,1);
uniform mat4 normalMatrix;

out vec2 texCoord; 
out vec3 normal; 
out vec3 worldPos; 

void main()
{ 
    texCoord = in_UV; 
    normal = (normalMatrix * vec4(in_Normal, 0.0)).xyz; 
    worldPos = (worldMatrix * vec4(in_Position, 1.0)).xyz;
	gl_Position = VPMatrix * vec4(in_Position, 1.0);
}