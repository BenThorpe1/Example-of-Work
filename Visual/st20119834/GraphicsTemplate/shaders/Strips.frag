#version 450 core
//////////////////////////////////////////////////////////////////////////////////////////
// Strips shader part of workshop.Applies a red ring around the object
//////////////////////////////////////////////////////////////////////////////////////////

layout(binding = 0) uniform sampler2D 	ColTexture;

#define PI 3.1415926538

out vec4 FragColor;	// Color that will be used for the fragment

in GS_DATA
{
  vec4 intensity;
  vec2 uv;
} inData;


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
	float ka = 0.2; // ambient coefficient
	float kd = 0.8; // diffuse coefficient

	vec4 intensity = clamp(inData.intensity, 0.0, 1.0);
	vec4 colour;
	
	float pos = inData.uv.y;
	float amountLine = 0.1;
	float lineThickness = 0.05;
	//Creates red rings
	float modValue = mod(pos,amountLine);
	
	if (modValue <lineThickness)
	{
	colour = vec4(1,0,0,0);
	}
	else
	{
	colour = vec4(1,1,1,1);
	}

	vec4 ambient = vec4(colour * ka);
	vec4 diffuse = vec4(colour * intensity) * kd;

	

	FragColor = vec4(ambient + diffuse);
}