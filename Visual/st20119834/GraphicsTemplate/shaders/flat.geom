#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_DATA
{
	vec2 uv;
} inData[];

out GS_DATA
{
 vec2 uv;
} outData;


void main() 
{
    for( int i=0; i<3; i++ )
    {
        // pass through
        outData.uv = inData[i].uv;
        gl_Position = gl_in[i].gl_Position; 
        EmitVertex();
    }  
    EndPrimitive();
}
