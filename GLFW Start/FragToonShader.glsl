#version 400 

out vec4 FragColor;

in vec4 GPosition;
in vec3 GNormal;
flat in int GIsEdge; // Whether or not we're drawing an edge

uniform vec3 Kd = vec3(1.0f,0.0f,1.0f); // Diffuse reflectivity
uniform vec3 LightPosition = vec3(1.0f,1.0f,1.0f);
uniform vec4 LineColor = vec4(1.0f,1.0f,1.0f,1.0f); // The sil. edge color
uniform mat4 ViewMatrix;

const int levels = 5;
const float scaleFactor = 1.0 / levels;

vec4 eyeLightPosition = ViewMatrix * vec4(LightPosition,1.0f);

vec3 toonShade( )
{
	vec3 s = normalize( eyeLightPosition - GPosition ).xyz;
	float cosine = max( 0.0, dot( s, normalize(GNormal) ) );
	vec3 diffuse = Kd * floor( cosine * levels ) * scaleFactor;
	return (diffuse);
}

void main() 
{
	if(GIsEdge==0)
	{
		FragColor = vec4(toonShade(), 1.0);
	}
	else
	{
		FragColor = LineColor;
	}
	//FragColor = vec4(1.0, 0.0, 1.0, 1.0);
	//FragColor = vec4(Normal,1.0)*vec4(1.0, 0.5, 0.5, 1.0);
}