#version 450 core

//////////////////////////////////////////////////////////////////////////////////////////
//Pass through shader
//////////////////////////////////////////////////////////////////////////////////////////

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_DATA
{
    vec2 uv;
	vec2 screenSize;
} inData[];

out GS_DATA
{
 vec2 uv;
	vec2 screenSize;
}outData;


void main()
{
	for(int i=0; i <3; i++)
	{
	outData.screenSize = inData[i].screenSize;
	outData.uv = inData[i].uv;
	gl_Position = gl_in[i].gl_Position;
	EmitVertex();
	}
	    EndPrimitive();

}
