#version 450 core

layout(binding = 0) uniform sampler2D 	ColTexture;

#define PI 3.1415926538

out vec4 FragColor;	// Color that will be used for the fragment

in GS_DATA
{
  vec4 intensity;
  vec2 uv;
} inData;

void main()
{
	vec4 colour = texture( ColTexture, inData.uv );
	colour = vec4(255,255,255,1);
	
	FragColor = vec4(colour);
}