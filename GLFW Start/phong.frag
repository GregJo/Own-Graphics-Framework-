// blinn-phong shading

#version 130

// use double precision if available
#ifdef ARB_gpu_shader_fp64
	#define vec dvec
	#define float double
#endif

uniform float gamma;
uniform bool isColor;

in vec3 es_normal;
in vec3 es_position;
in vec3 rgbColor;
in vec3 lightPos;
in vec3 infiniteView;
in float directional_light;


float C_NORMALIZATION = 1.0 / 100.0;

// brdf-normalized blinn-phong shading (half vector based)
vec4 blinn_phong(vec4 ambient_color, vec4 diffuse_color, vec4 specular_color,
				 vec3 es_pos, vec3 es_n, vec3 es_lightPos, float shiny, float directional )
{
	 //vec3 view = normalize(-es_pos);
	 // the "infinite viewer": actually good for contrast, but less realistic
	 vec3 view = infiniteView;
	 vec3 light = normalize(es_lightPos - es_pos * directional);
	 vec4 diffuseResult = diffuse_color* max(0.0, dot(light, es_n));
 	 
	 vec3 halfVec = normalize(light + view);
	 // BRDF normalization term
	 // lighting model takes energy conservation into account (overall brightness of highlight decreases with smoothness)
	 float specular_fraction = (shiny == 0.0) ? 0.0 : pow( max(0.0, dot(halfVec, es_n)), shiny );// * (shiny + 8.0) * C_NORMALIZATION;
	 vec4 specularResult = specular_color * specular_fraction;
	 return ambient_color + diffuseResult + specularResult;
}

// gamma correction
vec3 gammaCorrect(vec3 inColor, float gamma)
{
	if(gamma == 0.0)
		return vec3(1.0);
		
	vec3 gammaRGB = vec3( pow( inColor.r, gamma ),
						  pow( inColor.g, gamma ),
						  pow( inColor.b, gamma ) );
	return gammaRGB;
}

void main() 
{
	vec4 fragColor;
	// diffuse shading for assembly view
	if( isColor )
	{
		fragColor = blinn_phong( vec4(0.0), vec4( rgbColor, 1.0 ), vec4(0.0),
								 es_position, normalize(es_normal), lightPos, 0.0, 0.0 );
	}
	//  full shading
	else
	{	
		fragColor = blinn_phong( gl_FrontMaterial.ambient, gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse, gl_FrontMaterial.specular,
								 es_position, normalize(es_normal), lightPos, gl_FrontMaterial.shininess, directional_light );
		// apply gamma correction
		fragColor.rgb = gammaCorrect(fragColor.rgb, gamma);
	}
								
	gl_FragColor = fragColor;
}
