#version 450 core

//////////////////////////////////////////////////////////////////////////////////////////
// Fog shader part of workshop. Creates a fog effect Not Finished
//////////////////////////////////////////////////////////////////////////////////////////

layout(binding = 0) uniform sampler2D 	ColTexture;

out vec4 FragColor;	// Color that will be used for the fragment

in GS_DATA
{
    float distance;
	vec4 intensity;
    vec2 uv;
} inData;

float CalculateFog(float value)
{
const float fogMax =200.0;
const float fogMin = 100.0;

if (value > fogMax)
{
return 1;
}

if (value < fogMin)
{
return 0.5;
}
return 1 - (fogMax- value) / (fogMax - fogMin);

}


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
	float ka = 0.1; // ambient coefficient
	float kd = 0.1; // diffuse coefficient

	//Band the intensity into 4 groups
	vec4 intensity = clamp(inData.intensity, 0.0, 1.0);
	

	vec4 colour = texture( ColTexture, inData.uv );
	vec4 ambient = vec4(colour * ka);
	vec4 diffuse = vec4(colour * intensity) * kd;

	float fogColour = CalculateFog(inData.distance);
	FragColor = vec4(ambient + diffuse + fogColour);

	
}
