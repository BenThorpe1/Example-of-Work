#version 450 core


//Pass through shader


layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_DATA
{
vec4 intensity;
vec4 light; 
	vec2 uv;
} inData[];

out GS_DATA
{
vec4 intensity;
vec4 light; 
 vec2 uv;
} outData;


void main()
{
	for(int i=0; i <3; i++)
	{
	outData.uv = inData[i].uv;
	outData.intensity = inData[i].intensity;
	outData.light = inData[i].light;
	gl_Position = gl_in[i].gl_Position;
	EmitVertex();
	}
	    EndPrimitive();

}