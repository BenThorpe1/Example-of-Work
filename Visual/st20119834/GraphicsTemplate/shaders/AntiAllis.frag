#version 450 core

//////////////////////////////////////////////////////////////////////////////////////////
//Prefroms a anti allis effect
//////////////////////////////////////////////////////////////////////////////////////////

layout(binding = 0) uniform sampler2D 	ColTexture;
layout(binding = 1) uniform sampler2D	ScreenGrabTexture;

out vec4 FragColor;	// Color that will be used for the fragment

in GS_DATA
{
    vec2 uv;
	vec2 screenSize;
} inData;

//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
	//Get all niebouring pixels
	//	0	1	2
	//	3	4	5
	//	6	7	8

	vec2 pixels[9];
	//Current Pixel
	pixels[4] = inData.uv;
	//The space until next nieght uv coordinate
	float xD = 1/inData.screenSize.x;
	float yD = 1/inData.screenSize.y;

	//Nighebouring pixels
	pixels[0] = vec2(pixels[4] + vec2(-xD,-yD));
	pixels[1] = vec2(pixels[4] + vec2(0,-yD));
	pixels[2] = vec2(pixels[4] + vec2(xD,-yD));
	pixels[3] = vec2(pixels[4] + vec2(-xD,0));
	pixels[5] = vec2(pixels[4] + vec2(xD,0));
	pixels[6] = vec2(pixels[4] + vec2(-xD,yD));
	pixels[7] = vec2(pixels[4] + vec2(0,yD));
	pixels[8] = vec2(pixels[4] + vec2(xD,yD));

	//Get all colour value for all pixels
	vec4 colours[9];
	//Sum up all the colours
	vec4 sum;
	for (int i = 0; i < 9; i++)
	{
	colours[i] = texture(ScreenGrabTexture, pixels[i]);
	sum += colours[i];
	}

	//Apply a mean filter
	sum /= 9;

	FragColor = vec4(vec3(sum),1.0);

	
}
