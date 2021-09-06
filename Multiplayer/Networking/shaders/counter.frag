#version 450 core

layout(binding = 0) uniform sampler2D 	ColTexture;

out vec4 FragColor;	// Color that will be used for the fragment

in VS_DATA
{
  vec2 uv;
} inData;


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
	vec4 colour = texture( ColTexture, inData.uv );

	FragColor = vec4(colour);
}