#version 450 core
//////////////////////////////////////////////////////////////////////////////////////////
// Halftone shader not complete
//////////////////////////////////////////////////////////////////////////////////////////

layout (location = 0) in vec3               vertexPos;	  	// Position in attribute location 0
layout (location = 1) in vec2               texCoord;
layout (location = 2) in vec3               vertexNormal;

layout (location = 3) uniform mat4          viewMatrix;
layout (location = 4) uniform mat4          projectionMatrix;
layout (location = 5) uniform mat4          modelMatrix;
layout (location = 6) uniform mat3          normalMatrix;
layout (location = 7) uniform vec3          LightPosition;

//Setup instance
struct objInstance
{
	vec4 pos;
    vec3 scale;
};

layout (std430, binding = 0) buffer InstanceBlock 
{
	objInstance instance[];
};


out VS_DATA
{
   vec2 size;
    vec2 uv;
 } outData;
 
//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
  
    vec4 pos = vec4(vertexPos,1.0) + instance[gl_InstanceID].pos;

  vec4 position = modelMatrix * pos;
   gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;  

    vec3 Normal = normalMatrix * vec3(vertexNormal.xyz);
    Normal = normalize(Normal);
    vec3 LightDir = vec3(LightPosition - vec3(position.xyz));
    vec3 L = normalize(LightDir);
	float d = dot(Normal, L);

  //Set the scale of the windows
    float screenW = 800;
    float screenH = 600;

    //Set the scale of each square
 //float screenScaleX = ((1.0f/10000.0f) * screenW);
 //float screenScaleY = ((1.0f/10000.0f) * screenH);
    
    //Set the scale of each square
float screenScaleX = ((1.0f/10.0f) * screenW);
 float screenScaleY = ((1.0f/10.0f) * screenH);
  
    
 //Calucalute the new uv coords
  vec2 pixel = texCoord;   
  
  //Limits for the display screen
  float MAX_WIDTH = screenW;
 float MAX_HEIGHT = screenH;

 //Detemines the differnce in pixel coordinats
 float xD = 1/screenW;
float yD = 1/screenH;

 //Determines where to start
 float LEFT =0 + (gl_InstanceID * 10 * xD);
 float TOP = 0 + (gl_InstanceID * 10* yD);
 //Determines how much of the screen to display
 float WIDTH =screenScaleX;
 float HEIGHT =screenScaleY;
 
 pixel.x = texCoord.x * (WIDTH/MAX_WIDTH)+(LEFT/MAX_WIDTH);
 pixel.y = texCoord.y * (HEIGHT/MAX_HEIGHT)+(TOP/MAX_HEIGHT);
 
 pixel = texCoord;


 float dC = distance(gl_Position,position);
 outData.uv = pixel;
 outData.size = vec2(screenW,screenH);
}