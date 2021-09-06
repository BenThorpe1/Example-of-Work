#version 450 core

struct objInstance
{
    vec2 pos;
	vec2 sprite_uv;
};

layout (std430, binding = 0) buffer InstanceBlock 
{
	objInstance instance[];
};

layout (location = 0) in vec3               vertexPos;	  	// Position in attribute location 0
layout (location = 1) in vec2               texCoord;
layout (location = 2) in vec3               vertexNormal;

layout (location = 4) uniform int           alphBlending;
layout (location = 5) uniform mat4          modelMatrix;


out VS_DATA
{
    flat int alpha;
    vec2 uv;
 } outData;


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
    // 2D screen position - i.e. no perspective added
	vec4 pos = vec4(vec3(vertexPos.xyz), 1.0);
    pos.x += ((float(gl_InstanceID+ instance[gl_InstanceID].pos.x)*2.0) );
    gl_Position = modelMatrix * pos;
    
    // assumes we are using a sprite map that is 16x16 characters in size. 
    // Needs changing to reflect the ascii character set
    vec2 step_size = vec2(1.0/16.0, 1.0/8.0); 
    
    // get character value here..
    vec2 s_uv = vec2(texCoord + instance[gl_InstanceID].sprite_uv);
    outData.uv = s_uv * step_size;
    outData.alpha = alphBlending;
}