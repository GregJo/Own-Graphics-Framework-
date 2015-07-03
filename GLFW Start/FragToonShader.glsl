#version 400 

out vec4 FragColor;

in vec2 GUV;
in vec4 GPosition;
in vec3 GNormal;
flat in int GIsEdge; // Whether or not we're drawing an edge

uniform sampler2D diffuse_Sampler;
uniform vec3 Kd = vec3(1.0,1.0,1.0); // Diffuse reflectivity
uniform vec3 Ks = vec3(1.0,1.0,1.0); // Specular reflectivity
uniform float Shininess = 100.0; // Specular shininess factor
uniform vec3 camPosition = vec3(0.0,0.0,0.0);
uniform vec3 lightPosition = vec3(0.0,0.0,0.0);
uniform vec4 LineColor = vec4(0.5,0.5,0.5,1.0); // The sil. edge color
uniform mat4 viewMatrix;

const int levels = 5;
const float scaleFactor = 1.0 / (levels+1);

vec4 eyeLightPosition = viewMatrix * vec4(lightPosition,1.0f);
vec4 eyeCamPosition = viewMatrix * vec4(camPosition,1.0f);

// Specular correction factor
float C_NORMALIZATION = 1.0 / 30.0;

vec3 toonShade( )
{
	vec4 texColor = texture( diffuse_Sampler, GUV );
	vec3 n = normalize( GNormal );
	vec3 s = normalize( (eyeLightPosition - GPosition).xyz );
	vec3 v = normalize( vec3(eyeCamPosition - GPosition) );
	vec3 h = normalize( v + s );
	
	float cosine = max( 0.0, dot( s, n ) );

	vec3 diffuse = Kd * max( dot(s, n), 0.0 );// * floor( cosine * levels ) * scaleFactor;
	vec3 specular = Ks * pow( max( dot( h, n ), 0.0 ), Shininess ) * ( Shininess + 8.0 ) * C_NORMALIZATION ;
	vec3 shadedTexColor = (texColor.xyz * floor( cosine * levels ) * scaleFactor);

	return ( shadedTexColor * ( diffuse + specular ) ) * floor( cosine * levels ) * scaleFactor;
}

void main() 
{
	if(GIsEdge==0)
	{
		FragColor = vec4( toonShade(), 1.0 );
		//FragColor = vec4( 1.0, 0.0, 1.0, 1.0 );
	}
	else
	{
		FragColor = LineColor;
	}
}