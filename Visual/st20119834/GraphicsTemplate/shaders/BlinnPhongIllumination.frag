//Causes errors bescuase of verison
//////////////////////////////////////////////////////////////////////////////////////////
// Blinn Phong shader not complete
//////////////////////////////////////////////////////////////////////////////////////////



#version 450 core

layout(binding = 0) uniform sampler2D 	ColTexture;
layout(binding = 1) uniform sampler2D	ScreenGrabTexture;

out vec4 FragColor;	// Color that will be used for the fragment

in GS_DATA
{
vec4 intensity;
vec4 light; 
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
	//vec4 lightIntensity = clamp(inData.light,0.0,1.0);
	vec4 lightIntensity = inData.light;

	vec4 colour = texture( ScreenGrabTexture, inData.uv );

	vec4 ambient = vec4(colour * ka);
	vec4 diffuse = vec4(colour * lightIntensity) * kd;
	
	FragColor = vec4(ambient + diffuse);
	
}