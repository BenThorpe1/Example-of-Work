#version 450 core

//////////////////////////////////////////////////////////////////////////////////////////
// GrayScale shader part of workshop.Sets the colour to gray
//////////////////////////////////////////////////////////////////////////////////////////

layout(binding = 0) uniform sampler2D 	ColTexture;
layout(binding = 1) uniform sampler2D	ScreenGrabTexture;

out vec4 FragColor;	// Color that will be used for the fragment

in GS_DATA
{
    vec2 uv;
} inData;

//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
	vec4 screen = texture(ScreenGrabTexture, inData.uv);
	vec4 colour = texture(ColTexture, inData.uv );

	float grayScale = colour.x *  0.299 + colour.y * 0.587 + colour.z * 0.144;

	FragColor = vec4(vec3(grayScale + screen),1.0);

	
}
