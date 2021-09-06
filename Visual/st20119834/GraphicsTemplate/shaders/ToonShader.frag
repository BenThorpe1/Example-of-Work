#version 450 core

//////////////////////////////////////////////////////////////////////////////////////////
// Toon shader part of workshop. Limites the intisty into brands
//////////////////////////////////////////////////////////////////////////////////////////

layout(binding = 0) uniform sampler2D 	ColTexture;

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

	//Band the intensity into 4 groups
	vec4 intensity = clamp(inData.intensity, 0.0, 1.0);
	//intensity.x = LimitValue(intensity.x);
	//intensity.y = LimitValue(intensity.y);
	//intensity.z = LimitValue(intensity.z);
	//intensity.w = LimitValue(intensity.w);

	vec4 colour = texture( ColTexture, inData.uv );
	vec4 ambient = vec4(colour * ka);
	vec4 diffuse = vec4(colour * intensity) * kd;

	FragColor = vec4(ambient + diffuse);
}
