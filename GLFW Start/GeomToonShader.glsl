#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 30 ) out;

out vec4 GPosition;
out vec3 GNormal;

flat out int GIsEdge;

in vec4 Position[];
in vec3 Normal[];

uniform float EdgeWidth = 0.005f; // Width of sil. edge in clip cds.
uniform float PctExtend = 0.02f; // Percentage to extend quad

bool isBackfacing( vec3 a, vec3 b, vec3 c )
{
	float crossProd = (a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y);
	return (!(crossProd > 0.0));
}

void emitEdgeQaud( vec3 e0, vec3 e1 )
{
	vec2 ext = PctExtend * (e1.xy - e0.xy);
	vec2 v = normalize(vec2(e1.x-e0.x,e1.y-e0.y));
	vec2 n = vec2(-v.y, v.x) * EdgeWidth;

	// Emit the quad
	GIsEdge = 1; // This is part of the sil. edge
	gl_Position = vec4( e0.xy - ext, e0.z, 1.0 );
	EmitVertex();
	gl_Position = vec4( e0.xy - n - ext, e0.z, 1.0 );
	EmitVertex();
	gl_Position = vec4( e1.xy + ext, e1.z, 1.0 );
	EmitVertex();
	gl_Position = vec4( e1.xy - n + ext, e1.z, 1.0 );
	EmitVertex();
	EndPrimitive();
}

void emitQuadOnBackfacingTriangle( vec3 a, vec3 b, vec3 c )
{
	emitEdgeQaud( b, a );
	emitEdgeQaud( c, b );
	emitEdgeQaud( a, c );
}

void main() 
{
	vec3 p0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
	vec3 p1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
	vec3 p2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;

	if( isBackfacing(p0, p1, p2) ) 
	{
		emitQuadOnBackfacingTriangle(p0, p1, p2);
	}

	GIsEdge = 0;
	
	GPosition = Position[0];
	GNormal = Normal[0];	
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

	GPosition = Position[1];
	GNormal = Normal[1];	
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

	GPosition = Position[2];
	GNormal = Normal[2];	
	gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}