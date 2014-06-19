#version 400 

in vec2 ex_UV;
in vec3 ex_vert_pos;
in vec3 ex_light_pos;
in vec3 ex_Normal;

out vec4 FragColor;

uniform sampler2D diffuse_Sampler;

layout (std140) uniform Material 
{
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    vec4 emissive;
    float shininess;
    //int texCount;
};
 
void main()
{
	vec3 normalized_normal = normalize(ex_Normal);
	vec3 normalized_vertex_to_light_vector = normalize(ex_light_pos-ex_vert_pos);
    
	float diffuseTerm = clamp(dot(normalized_normal, normalized_vertex_to_light_vector), 0.0, 1.0);
	
	float dot_prod_specular = dot (normalized_vertex_to_light_vector, normalized_normal);
	float specular_factor = pow (dot_prod_specular, 50);//shininess);

	vec4 diffuse_color  = vec4((diffuse.rgb * (texture( diffuse_Sampler, ex_UV ).rgb * diffuseTerm)),1);
	vec4 specular_color = vec4((specular.rgb * specular_factor), 1);
	vec4 final_color    = vec4(0.2*ambient.rgb,1) + ambient * diffuse_color + specular_color + 0 * vec4(emissive.rgb,1); 

	FragColor = final_color;
	//FragColor = vec4((vec3(0.5,0.5,0.5) * specular_factor), 1); //vec3(1.0,0.5,0.0)
	//FragColor = specular;
}