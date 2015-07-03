#version 420

in vec2 UV;

//uniform sampler2D positionMap;
layout( binding = 0 ) uniform sampler2D colorMap;
layout( binding = 1 ) uniform sampler2D normalMap;

uniform float intensity = 0.30f;
uniform vec2 screenSize = vec2(1200.0,800.0);
uniform vec3 lightDir = vec3(-1.0f,-1.0f,1.0f);

layout( location = 0 ) out vec4 FragColor;

vec2 CalcTexCoord()
{
   return UV / screenSize;
   //return gl_FragCoord.xy / screenSize;
}

float CalcDirectionalLight(vec3 lightDir, vec3 normal )
{
	float dirLightCoeff = 0;
	dirLightCoeff = max( dot(normalize(lightDir), normal), 0.0 );
	return dirLightCoeff;
}

void main()
{
   	vec3 color = texture(colorMap, UV).xyz;
   	vec3 normal = texture(normalMap, UV).xyz;
   	normal = normalize(normal);

   	FragColor =  intensity * CalcDirectionalLight(lightDir,normal) * vec4(color, 1.0);//+vec4(0.2);//*ceil(abs(dot(normalize(worldPos), normalize(vec3(1.0)))));
	//FragColor = vec4(0.5);
	//FragColor = vec4(normal, 1.0);
}