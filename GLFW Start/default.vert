// default vertex shader, usable with multiple fragment programs from DefMap viewer update

// use double precision if available
#ifdef ARB_gpu_shader_fp64
	#define vec dvec
	#define float double
#endif

attribute vec3 in_Position;
attribute vec3 in_Normal;
//attribute int in_Color;

varying vec3 es_normal;
varying vec3 es_position;
varying vec3 lightPos;
// constant light position (DEBUG/contrast highlight setting)
vec4 lightP  = vec4(0.0, 3000000.0, 5000000.0, 1.0);

void main()
{
	vec4 homogenousPosition = gl_ModelViewMatrix * vec4(in_Position,1);
	es_position = homogenousPosition.xyz / homogenousPosition.w;
	es_normal = normalize(gl_NormalMatrix * in_Normal);
	//lightPos = lightP.xyz;
	lightPos = (gl_ModelViewMatrix * gl_LightSource[0].position).xyz;

	gl_Position = gl_ProjectionMatrix * homogenousPosition;
}