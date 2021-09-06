#version 450 core

//Pass through shader

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_DATA
{
   vec2 size;
    vec2 uv;
} inData[];

out GS_DATA
{
  vec2 size;
    vec2 uv;
} outData;


void main()
{
	for(int i=0; i <3; i++)
	{
	outData.size = inData[i].size;
	outData.uv = inData[i].uv;
	gl_Position = gl_in[i].gl_Position;
	EmitVertex();
	}
	    EndPrimitive();

}