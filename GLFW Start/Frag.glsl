//in vec2 UV;
//in vec3 eye_vert_pos;
//in vec3 light_pos;
//in vec3 normal;

//uniform sampler2D diffuse_Sampler;

//layout (std140) uniform Material 
//{
//    vec4 diffuse;
//    vec4 ambient;
//    vec4 specular;
//    vec4 emissive;
//    float shininess;
//    //int texCount;
//};

#version 400

in vec4 Position;
in vec3 Normal;
in vec4 eyeLightPosition;

uniform vec3 LightIntensity = vec3(1.0f,1.0f,1.0f);
uniform vec3 Kd = vec3(1.0f,0.5f,0.0f); // Diffuse reflectivity
uniform vec3 Ka = vec3(0.0f,0.0f,0.0f); // Ambient reflectivity
uniform vec3 Ks = vec3(1.0f,0.5f,0.0f); // Specular reflectivity
uniform float Shininess = 200.0f; // Specular shininess factor

layout( location = 0 ) out vec4 FragColor;

float C_NORMALIZATION = 1.0 / 30.0;

vec3 ads()
{
	vec3 n = normalize( Normal );
	vec3 s = normalize( vec3(eyeLightPosition) - vec3(Position) );
	vec3 v = normalize( vec3(-Position) );
	vec3 h = normalize( v + s );
	
	return LightIntensity * ( Ka + Kd * max( dot(s, n), 0.0 ) + Ks * pow(max(dot(h,n),0.0), Shininess ) * (Shininess + 8.0) * C_NORMALIZATION );
}

void main() 
{
	FragColor = vec4(ads(), 1.0);
}