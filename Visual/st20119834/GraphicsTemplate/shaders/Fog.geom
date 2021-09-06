#version 450 core


layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_DATA
{
   float distance;
	vec4 intensity;
    vec2 uv;
} inData[];

out GS_DATA
{
  float distance;
	vec4 intensity;
    vec2 uv;
} outData;


void main()
{
	for(int i=0; i <3; i++)
	{
	outData.distance = inData[i].distance;
	outData.intensity = inData[i].intensity;
	outData.uv = inData[i].uv;
	gl_Position = gl_in[i].gl_Position;
	EmitVertex();
	}
	    EndPrimitive();

}