#version 420

in vec2 UV;

layout( binding = 0 )uniform sampler2D positionMap;
layout( binding = 1 )uniform sampler2D colorMap;
layout( binding = 2 )uniform sampler2D normalMap;

//uniform sampler2D positionMap;
//uniform sampler2D colorMap;
//uniform sampler2D normalMap;

uniform float lightSphereRadius = 1.0f;
uniform vec2 screenSize = vec2(1200.0,800.0);
uniform vec3 lightPosition = vec3(10.0f,10.0f,10.0f);
uniform vec3 lightColor = vec3(1.0f,0.0f,1.0f);
uniform vec3 camPosition = vec3(10.0f,10.0f,10.0f);

layout( location = 0 ) out vec4 FragColor;

float diffuseIntensity = 1.5f;
float specularIntensity = 1.0f;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / screenSize;
}

void main()
{
	vec2 UVTexCoord = CalcTexCoord();

	vec3 position = texture(positionMap, UVTexCoord).xyz;
	//vec3 position2 = texture(positionMap, 10*UV).xyz;
	vec3 color = texture(colorMap, UVTexCoord).xyz;
	vec3 normal = texture(normalMap, UVTexCoord).xyz;
	normal = normalize(normal);
	//vec3 normal2 = texture(normalMap, 10*UV).xyz;
	
	vec3 lightDirection = position - lightPosition;
    float distance = abs(length(lightDirection));
    lightDirection = normalize(lightDirection);

	vec3 cameraDirection = normalize( vec3( position - camPosition ) );
	cameraDirection = normalize( cameraDirection );
	vec3 halfWayVec = normalize( ((-lightDirection) + (-cameraDirection)) );
	//vec3 reflectionVector = normalize(-lightDirection - 2 * dot(-lightDirection, normal) * normal); 

	float diffuseFactor = 0.0;
	float specularFactor = 0.0;

	if (dot(-lightDirection, normal) > 0.0)
	{
		diffuseFactor = max(dot(normal, -lightDirection), 0.0);
		specularFactor = pow(max(dot(normal, halfWayVec), 0.0), 64.0 ) * (64.0 + 8.0) * 1.0/30.0;
		//specularFactor = (pow(max(dot( reflectionVector, cameraDirection), 0.0), 200.0 ) * (200.0 + 8.0) * 1.0/30.0);
	}

	float fallOff = clamp(((lightSphereRadius - distance)/lightSphereRadius),0.0,1.0);
	fallOff *= fallOff;

	//FragColor = vec4(color, 1.0) * (diffuseFactor * vec4(lightColor, 1.0) * fallOff);// + vec4(0.1,0.1,0.1,0.0);// / (Distance * Distance);
	FragColor = vec4(0.15, 0.15, 0.15, 0.0) * (diffuseFactor * diffuseIntensity * vec4(lightColor, 0.0) * fallOff) + vec4(color, 1.0) * (diffuseFactor * diffuseIntensity * vec4(lightColor, 1.0) * fallOff) + vec4(color, 0.0) * specularFactor * specularIntensity * vec4(lightColor, 0.0) * fallOff;
	//FragColor = specularFactor * vec4(lightColor, 1.0) * fallOff;
	//FragColor = vec4(1.0/2.0*color, 1.0);
	//FragColor = vec4(normalize(normal),1.0);
	//FragColor = vec4(0.5);
}