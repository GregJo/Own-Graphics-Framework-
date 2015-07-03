#version 400

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

uniform vec3 camPosition = vec3(0.0,0.0,0.0);
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;
//uniform mat4 projectionMatrix;
uniform mat4 VPMatrix;
uniform float nPlane = 0.01;
uniform float fPlane = 10000.0;

out float Scale;
out vec2 UV;
out vec4 Position;
out vec3 Normal;

void main()
{
	//vec4 tnorm = normalize(NormalMatrix * vec4(in_Normal,1.0));
	UV = in_UV;
	Position = viewMatrix * vec4(in_Position,1.0);
	Scale = 1.0-(abs(camPosition.z-in_Position.z)/fPlane);//nPlane/max(nPlane,abs((CamPosition.z-Position.z)/fPlane));
	//Normal = (vormalMatrix * vec4(in_Normal, 0.0)).xyz;
	Normal = (viewMatrix * vec4(in_Normal, 0.0)).xyz;

	gl_Position = VPMatrix * vec4(in_Position,1.0f);
}