#version 450 core
//////////////////////////////////////////////////////////////////////////////////////////
// Toon shader part of workshop. Limites the intisty into brands
//////////////////////////////////////////////////////////////////////////////////////////

layout (location = 0) in vec3               vertexPos;	  	// Position in attribute location 0
layout (location = 1) in vec2               texCoord;
layout (location = 2) in vec3               vertexNormal;

layout (location = 3) uniform mat4          viewMatrix;
layout (location = 4) uniform mat4          projectionMatrix;
layout (location = 5) uniform mat4          modelMatrix;
layout (location = 6) uniform mat3          normalMatrix;
layout (location = 7) uniform vec3          LightPosition;


out VS_DATA
{
    vec4 intensity;
    vec2 uv;
 } outData;
 
//////////////////////////////////////////////////////////////////
//Limits the value of the intisity
//////////////////////////////////////////////////////////////////

 float LimitValue(float value)
{
	//1
	if (value > 0.875)
	{
	return 1;
	}
	else if (value > 0.625)
	{
	return 0.75;
	}
	else if (value > 0.375)
	{
	return 0.5;
	}
	else if (value > 0.125)
	{
	return 0.25;
	}
	else
	{
	return 0;
	}
}

//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
    //Standard shader information
	vec4 pos = vec4(vec3(vertexPos.xyz), 1.0);
	vec4 position = modelMatrix * pos;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;

    vec3 Normal = normalMatrix * vec3(vertexNormal.xyz);
    Normal = normalize(Normal);
    vec3 LightDir = vec3(LightPosition - vec3(position.xyz));
    vec3 L = normalize(LightDir);
	float d = dot(Normal, L);
	d = LimitValue(d);
    outData.intensity = vec4(vec3(d), 1.0);
    outData.uv = texCoord;    
}