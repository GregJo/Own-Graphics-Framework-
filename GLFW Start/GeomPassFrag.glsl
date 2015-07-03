#version 400 

in vec2 texCoord; 
in vec3 normal; 
in vec3 worldPos; 

layout (location = 0) out vec3 worldPosOut; 
layout (location = 1) out vec3 diffuseOut; 
layout (location = 2) out vec3 normalOut; 
layout (location = 3) out vec3 texCoordOut; 

uniform sampler2D colorMap; 

void main() 
{ 
    worldPosOut = worldPos; 
    diffuseOut = texture(colorMap, texCoord).xyz; 
    normalOut = normalize(normal).xyz; 
    texCoordOut = vec3(texCoord, 0.0); 
}