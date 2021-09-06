
//Example of pass though geomtry shader

// geometry shader - renders a solid triangle
#version 450
layout (triangles) in;	  	
//layout (line_strip, max_vertices = 4) out;
layout (points, max_vertices = 1)out;
in VS_DATA
{
  vec4 intensity;
  vec2 uv;
} inData[];

out GS_DATA
{
  vec4 intensity;
  vec2 uv;
} outData;


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main() 
{
//Wire Frame
//  for( int i=0; i<3; i++ )
//  {
//      // pass through
//      outData.intensity = inData[i].intensity; 
//      outData.uv = inData[i].uv;
//      gl_Position = gl_in[i].gl_Position; 
//      EmitVertex();
//  }
//  
//      // pass through
//      outData.intensity = inData[0].intensity; 
//      outData.uv = inData[0].uv;
//      gl_Position = gl_in[0].gl_Position; 
//      EmitVertex();
//
//      EndPrimitive();

//Point
	outData.intensity = inData[0].intensity;
	gl_Position = gl_in[0].gl_Position; 

	//Find center
	float cX = (inData[0].uv.x + inData[1].uv.x + inData[2].uv.x)/3;
	float cY = (inData[0].uv.y + inData[1].uv.y + inData[2].uv.y)/3;

	outData.uv = vec2(cX,cY);
	 EmitVertex();
    EndPrimitive();
}
