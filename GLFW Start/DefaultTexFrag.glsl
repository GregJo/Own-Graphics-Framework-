#version 400 

layout(location = 0) out vec4 FragColor;

in vec2 UV;
in vec4 Position;

uniform sampler2D renderTargetSampler;
uniform float time;

void main() 
{
	//FragColor = texture( renderTargetSampler, UV );
	vec4 blurredColor = vec4(0);
	for( int i = 0; i < 150 ; ++i )
	{
		float x = -(float(8)*float(1200.0))/2.0 + float(mod(i,8))*float(1200.0);
		float y = -(float(8)*float(800.0))/2.0 + float(i/8)*float(800.0);
		blurredColor += texture( renderTargetSampler, UV + 1*vec2(x,y) );
	}

	blurredColor = 1.0/150.0 * blurredColor;

	FragColor = blurredColor; //texture( renderTargetSampler, UV + 0.004*vec2( sin(time+1200.0*UV.x),cos(time+800.0*UV.y)) );
}