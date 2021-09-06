#version 450 core
layout (location = 0) in vec3               vertexPos;	  	// Position in attribute location 0
layout (location = 1) in vec2               texCoord;

layout (location = 2) uniform mat4          modelMatrix;
layout (location = 3) uniform vec2          offset;

out VS_DATA
{
    vec2 uv;
 } outData;


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
    vec2 step_size = vec2(0.1);
    vec2  sprite_uv = texCoord + offset;
    outData.uv = sprite_uv * step_size;

    // 2D screen position - i.e. no perspective added
	vec4 pos = vec4(vec3(vertexPos.xyz), 1.0);
    gl_Position = modelMatrix * pos;
}