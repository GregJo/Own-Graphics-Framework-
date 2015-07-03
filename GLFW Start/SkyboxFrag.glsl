#version 400

in vec2 UV;

uniform sampler2D skyBoxSampler;

layout( location = 0 ) out vec4 FragColor;

void main() 
{
	vec4 texColor = texture( skyBoxSampler, UV );
	FragColor = texColor;
	//FragColor = vec4(1.0,1.0,0.0,1.0);
}