#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

uniform vec3 lightPosition; // Light position in eye coords.
uniform vec3 camPosition = vec3(0.0,0.0,0.0);
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;
//uniform mat4 projectionMatrix;
uniform mat4 VPMatrix;

out vec2 UV;
out vec4 Position;
out vec3 Normal;
out vec4 eyeLightPosition;
out vec4 eyeCamPosition;

void main()
{
	//vec4 tnorm = normalize(NormalMatrix * vec4(in_Normal,0.0));
	UV = in_UV;
	Position = viewMatrix * vec4(in_Position,1.0);
	Normal = (normalMatrix * vec4(in_Normal, 0.0)).xyz;
	eyeLightPosition = viewMatrix * vec4(lightPosition,1.0f);
	eyeCamPosition = viewMatrix * vec4(camPosition,1.0);

	gl_Position = VPMatrix * vec4(in_Position,1.0f);
}