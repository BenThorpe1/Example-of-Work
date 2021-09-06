//////////////////////////////////////////////////////////////////////////////////////////
// Blinn Phong shader not complete
//////////////////////////////////////////////////////////////////////////////////////////


#version 450 core
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
vec4 light; 
  vec2 uv;
 } outData;


//////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////
void main()
{
 //Set the object position, Fills the screen with the texture
	vec4 pos = vec4(vec3(vertexPos.xyz), 1.0);
    
  float scaleX =3;
  float scaleY = 3;
 float scaleZ = 1;
  //Set the scale
 mat4 iModelMat = mat4(
            vec4(scaleX, 0.0f, 0.0f, 0.0f),
			  vec4(0.0f, scaleY, 0.0f, 0.0f),
			  vec4(0.0f, 0.0f, scaleZ, 0.0f),
			  vec4(pos.x, pos.y, pos.z, 1.0f) );
    iModelMat *= modelMatrix;
    
   vec4 position = iModelMat * pos;
   gl_Position = projectionMatrix * viewMatrix * iModelMat * pos;
     
 
 //Calculate normals and light directions
 vec3 normal = normalMatrix * vec3(vertexNormal.xyz);
 normal = normalize(normal);
 
 //Calucate light direction
 vec3 lightDir = vec3(LightPosition - vec3(position.xyz));
 lightDir = normalize(lightDir);
 
 //Calculate view Direction
 vec3 viewPos = viewMatrix[3].xyz;
 vec3 viewDir = vec3(viewPos - vec3(position.xyz));
 viewDir = normalize(viewDir);
 
 //Calculate halfway dir
 vec3 halfwayDir = (lightDir + viewDir);
 halfwayDir = normalize(halfwayDir);

//Calculate the shinnines   
 float shininess = 16.00f;
 
 float light =  max(dot(halfwayDir, normal), 0.0);
 light = pow(light, shininess);
 float distance = dot(normal, lightDir);

//Set outdata
outData.intensity = vec4(vec3(distance), 1.0);
outData.light = vec4(vec3(light),1.0);
outData.uv = texCoord;
}