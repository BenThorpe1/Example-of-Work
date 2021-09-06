// Includes
#include "core.h"
#include <math.h>
#include "cObjImport.h"
#include "cCamera.h"

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

using namespace glm;


//Constents
//Shader index to be used for multi render pass
const int RENDER_PASS_SECOND = 6;
const int RENDER_PASS_THRID = 11;
//Object index for the screen covering quad
const int SCREEN_QUAD = 2;
//Number of objects
const int NUMBER_OBJECTS =3;
//The texture index for the screen grab
const int SCREEN_TEXTURE =1;

//ShaderID
// 0 = Flat
// 1 = Diffuse
// 2 = Toon
// 3 = strips
// 4 = LightFalloff
// 5 = fog
// 6 = BlinnPhongIllumination
// 7 = GrayScale
//8 = RedScale
//9 = AntiAllis
//10 = Geomtrty
//11 = Halftone
// global variables

sMeshInfo meshInfo[] = {
	// name[32],			meshID,		shaderID			
	{"skybox.obj",			0,			0},
	{"torus.obj",			1,			2}, 
	{"FullScreenQuad.obj",		2,			0},
};

cObjImport gameObjects[NUMBER_OBJECTS];

cShaderInfo shaderInfo;

int currentMode = 0;
cCamera Camera;
//Whether or not we are using multi-pass rendering 
bool secondPass = false;
bool thirdPass = false;

//Instancing

struct alignas(16) objInstance
{
	alignas(16) glm::vec4 pos;
	alignas(16) glm::vec3 scale;
	objInstance(glm::vec4 pos, glm::vec3 scale)
	{
		this->pos = pos;
		this->scale = scale;
	}
};
//The vertex buffer object used to store all the instance data
GLuint	instanceVBO;
//The number of instances using
int numInstancesRow = 80; // 80 2
int numInstancesColum =80; //80 2
//Functions

// Resize the window, 
void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window
}

// Controlls the movement with keys. 
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	// ignore key up (key release) events
	if (action == GLFW_RELEASE) 
		return;			

	// process keyboard inputs here..
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_X:
			glfwSetWindowShouldClose(window, true);
		break;

		// very simple camera controls to help navigate the scene
		case GLFW_KEY_A:
		{
			// rotate camera about y-axis
			Camera.m_pan += 1.0f;
		}break;

		case GLFW_KEY_D:
		{
			// rotate camera about y-axis
			Camera.m_pan -= 1.0f;
		}break;

		case GLFW_KEY_W:
		{
			// rotate camera about x-axis
			Camera.m_tilt -= 1.0f;
		}break;

		case GLFW_KEY_S:
		{
			// rotate camera about x-axis
			Camera.m_tilt += 1.0f;
		}break;

		case GLFW_KEY_DOWN:
		{
			// zoom out along z-axis
			if (Camera.m_zoom > -14.0f)
				Camera.m_zoom -= 0.25f;
		}break;

		case GLFW_KEY_UP:
		{
			// zoom in along z-axis
			if(Camera.m_zoom < -0.25f )
				Camera.m_zoom += 0.25f;
		}break;

		case GLFW_KEY_SPACE:
		{
			secondPass = !secondPass;
		}break;
		
		case GLFW_KEY_TAB:
		{
			thirdPass = !thirdPass;
		}break;

		default:
		{
		}
	}
}

// Setup all objects.
void SetupGeometries()
{
	// import all mesh files here..
	for (int i = 0; i < NUMBER_OBJECTS; i++)
	{
		gameObjects[i].init(meshInfo[i]);
	}
	
	
	glBindVertexArray(0);
	check_for_opengl_errors();
}

// Initilise scenes. 
void init_SceneData()
{
	SetupGeometries();

	// load the list of shaders required within this scene
	shaderInfo.create();

	check_for_opengl_errors();
}

// Renders the scene. with objects shaders and textures
void renderScene()
{
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//check_for_opengl_errors();

	//Clear the back fuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Renders all models with thier defaul shaders
	for (int i = 0; i < NUMBER_OBJECTS; i++)
	{
		if (i == SCREEN_QUAD)
		{
			continue;
		}
		gameObjects[i].render(&shaderInfo);
	}

	//Clear the gpc bind array
	glBindVertexArray(0);       
//	check_for_opengl_errors();
}

// printDebugInfo() - print version info to console
void printDebugInfo()
{
	// Print info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

#ifdef GL_SHADING_LANGUAGE_VERSION
	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

	printf("Using GLEW version %s.\n", glewGetString(GLEW_VERSION));
	printf("------------------------------\n");
	printf("Press ESCAPE or 'X' or 'x' to exit.\n");
}

//Update all models for logic // Currently doesn't do anything important
void UpdateModel()
{
	gameObjects[1].rotate(vec3(1, 1, 0), 0.2f);	
	//gameObjects[1].move(vec3(0.01, 0.01, 0), 0.02);
}

//Creates the texture file for the screen grab
void CreateScreenTexture(GLFWwindow* windows,int index)
{
	// Create one OpenGL texture
	unsigned int textureID;
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height;
	glfwGetWindowSize(windows, &width, &height);
	
	glReadBuffer(GL_BACK);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
	glDrawBuffer(GL_BACK);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	gameObjects[index].AddTexture(textureID);
}

//Covert the back buffer to a texture
void ScreenToTexure(GLFWwindow* windows)
{
	//Get the windows sizes
	int width, height;
	glfwGetWindowSize(windows, &width, &height);
	//Convert the back buffer to the texture
	//Inform buffer that we are reading 
	glReadBuffer(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, gameObjects[SCREEN_QUAD].m_tex[SCREEN_TEXTURE]);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);

	//glBindTexture(GL_TEXTURE_2D, gameObjects[SCREEN_QUAD].m_tex[SCREEN_TEXTURE]
	glDrawBuffer(GL_BACK);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//Render the screen texture with shader
void RenderScreenTexture(int shaderID)
{

	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//check_for_opengl_errors();

	//Claer the back fuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Set the object to use the screen texture
	//glActiveTexture(GL_TEXTURE0 + gameObjects[SCREEN_QUAD].m_tex[SCREEN_TEXTURE]);
	
	//Render the screen quad with particular shader
	gameObjects[SCREEN_QUAD].renderShader(&shaderInfo, shaderID);

	//Clear the gpc bind array
	glBindVertexArray(0);      
	//check_for_opengl_errors();
}

//Render using multiple instances
void RenderHalftone(int shaderID)
{

//	check_for_opengl_errors();
	
	 //Clear the rendering window
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//check_for_opengl_errors();

	//Claer the back fuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Render the screen quad with particular shader
	gameObjects[SCREEN_QUAD].renderInstance(&shaderInfo, instanceVBO, numInstancesRow * numInstancesColum, shaderID);

}

//Create the matrix for instance array
void CreateInstanceArray()
{
	//Create the instance buffer object
	glCreateBuffers(1, &instanceVBO);

	glNamedBufferStorage(instanceVBO, int(numInstancesRow*numInstancesColum) * sizeof(objInstance), 0, GL_MAP_WRITE_BIT);

	//An array of all data within the instance buffer
	auto instanceArray = (objInstance*)glMapNamedBuffer(instanceVBO, GL_WRITE_ONLY);

	float maxX = 100; //200 2 100
	float maxY =100; //200 2 100
	float startX = -numInstancesRow;
	float startY = -numInstancesColum;
	//Calucalte the postional steps
	float xStep = float((maxX - startX)/ numInstancesRow);
	float yStep = float((maxY - startY)/ numInstancesColum);
	
	//Calulcate size //10 1
	float xSize = (10.0f / numInstancesRow);
	float ySize = (10.0f / numInstancesColum);

	//gameObjects[SCREEN_QUAD].scale(vec3(xSize, ySize, 1.0f));

	int i = 0;
			for (GLint x = 0; x < numInstancesRow; x++)
	{

		//Set each position of instance to be differences
		for (GLint y = 0; y < numInstancesColum; y++)
		{
			float xPos = startX + (xStep * x);
			float yPos = startY + (yStep * y);
			instanceArray[i] = objInstance(glm::vec4(xPos, yPos, 0.0, 1.0), glm::vec3(xSize, ySize, 1.0f));
			i++;
		}
	}

	// Once done un-map
	glUnmapNamedBuffer(instanceVBO);
}

// Main object loop
int main() 
{
	//Error Checks
	int count = sizeof(meshInfo) / sizeof(sMeshInfo);
	if(count != NUMBER_OBJECTS)
	{
		printf("\nERROR! Number of objects does not match the number of items contained in meshInfo[]\n\n");
		assert(0);
	}

	count = sizeof(gameObjects) / sizeof(cObjImport);
	if (count != NUMBER_OBJECTS)
	{
		printf("\nERROR! Number of objects does not match the number of items contained in OBJ[]\n\n");
		assert(0);
	}

	// initialise glfw
	glfwInit();

	//Creates the display window, the screen in which data will be drawn it
	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "GraphicsTemplate", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stdout, "Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// initialise glew
	glewInit();
	
	check_for_opengl_errors();

	// print version info 
	printDebugInfo();

	// Set callback function for resizing the window
	glfwSetFramebufferSizeCallback(window, resize);

	// Set callback for keyboard events
	glfwSetKeyCallback(window, keyboard);
	//Set the inital size of the windons
	resize(window, initWidth, initHeight);

	check_for_opengl_errors();

	// initialise geometry and shaders
	init_SceneData();
	
	check_for_opengl_errors();

	//Contains all shader information
	cShader* listShader = shaderInfo.getList();
	
	CreateScreenTexture(window, SCREEN_QUAD);

	check_for_opengl_errors();

	//Set the scale of each square
	CreateInstanceArray();

	   //Set the scale of the windows
	float screenW = 800;
	float screenH = 600;

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {
	
		//Updates the model information and camera logic
		UpdateModel();						
		Camera.update();
	
		//Render the screen
		//Render first pass of screen using defaul shaders and texture
		renderScene();
		if (secondPass)
		{
			//Get the screen as a texture
			ScreenToTexure(window);

			//Render second pass applying the chosen shader
			RenderScreenTexture(RENDER_PASS_SECOND);
		}

		if (thirdPass)
		{

			//Get the screen as a texture
			ScreenToTexure(window);
			//Render halftone shader
			RenderHalftone(RENDER_PASS_THRID);
		}

	// Displays what was just rendered (using double buffering).
		glfwSwapBuffers(window);			

		// Poll events (key presses, mouse events)
		//glfwWaitEvents();					// Use this if no animation.
		//glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();		// Use this version when animating as fast as possible
	//check_for_opengl_errors();

	}
	//End the programm 

	glfwTerminate();
	return 0;
}

//To delete

//
////Creates the texture used for buffer
//unsigned int CreateBufferTexture()
//{
//	unsigned int textureBuffer;
//	glGenTextures(1, &textureBuffer);
//	glBindTexture(GL_TEXTURE_2D, textureBuffer);
//	//Set the image
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, initWidth, initHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	//??? Needed for reason
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//Clear the current texture
//	glBindTexture(GL_TEXTURE_2D, 0);
//	//Apply texture to buffer
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureBuffer, 0);
//	return textureBuffer;
//}
//
////Create a buffer to render all the object
//unsigned int CreateRenderingObject()
//{
//	//Create rendering object
//	unsigned int renderingObject;
//	glGenRenderbuffers(1, &renderingObject);
//	glBindRenderbuffer(GL_RENDERBUFFER, renderingObject);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderingObject);
//
//	//Check that the buffer is successfully created
//	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	//	printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	return renderingObject;
//}
//
////Create a vertex encasplating the entire screen
//unsigned int CreateSceneQuad(float quadVertices[])
//{
//	unsigned int VAO;
//	unsigned int VBO;
//
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER,VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//	return VAO;
//}
//

//Delete Test
////View Direction
//vec3 viewPos = vec3(viewMatrix.xyz);
//vec3 viewDir = vec3(viewPos - vec3(position.xyz))
//vec3 viewNormal = normalize(viewDir);
//
////Halfway vector
//vec3 Half = lightNormal + viewNormal;
//vec3 HalfNormal = normalize(Half);

//
//	//Sets the position of the object
//vec4 pos = vec4(vec3(vertexPos.xyz), 1.0);
//vec4 position = modelMatrix * pos;
//gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;
//
////Calculate lighting effects. 
//vec3 Normal = normalMatrix * vec3(vertexNormal.xyz);
//Normal = normalize(Normal);
////Light direction
//vec3 LightDir = vec3(LightPosition - vec3(position.xyz));
//vec3 LightNormal = normalize(LightDir);
//
//
////Speclication value
//
//
//float distance = dot(Normal, LightNormal);
//
//
////Set out data
//outData.intensity = vec4(vec3(distance), 1.0);
//outData.uv = texCoord;
//
//#version 450 core
//
////Calculates lighting effects
//
//layout(binding = 0) uniform sampler2D 	ColTexture;
//
//out vec4 FragColor;	// Color that will be used for the fragment
//
//in VS_DATA
//{
//  vec4 intensity;
//  vec2 uv;
//} inData;

//
////////////////////////////////////////////////////////////////////
//// main()
////////////////////////////////////////////////////////////////////
//void main()
//{
//	float ka = 0.2; // ambient coefficient
//	float kd = 0.8; // diffuse coefficient
//
//	vec4 intensity = clamp(inData.intensity, 0.0, 1.0);
//
//	vec4 colour = texture(ColTexture, inData.uv);
//
//	vec4 ambient = vec4(colour * ka);
//	vec4 diffuse = vec4(colour * intensity) * kd;
//
//
//
//	FragColor = vec4(ambient + diffuse);
//}
//
//
//void cObjImport::renderShader(cShaderInfo* pShaderInfo, unsigned int index, GLuint textureID)
//{
//	cShader* pList = pShaderInfo->getList();
//	// enable shader program..
//	glUseProgram(pList[index].program());
//
//	//	modelMatrix = glm::rotate(mat4(1.0f), radians(0.0f), vec3(0, 1, 0));
//	normalMatrix = transpose(inverse(modelMatrix));
//
//	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(Camera.viewMatrix));
//	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(Camera.projectionMatrix));
//	glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(modelMatrix));
//	glUniformMatrix3fv(6, 1, GL_FALSE, glm::value_ptr(normalMatrix));
//	glUniform3fv(7, 1, glm::value_ptr(LightPosition));
//
//	// select which texture we want to use..
//	glBindTextureUnit(0, m_tex[textureID]);
//	//	glBindTextureUnit(1, m_tex[1]); // reflection map
//
//		// Draw FACES
//	int vertexCount = m_f_count * 3; // calculate the number of vertices for this object
//	glBindVertexArray(m_VAO);// myVAO[objID[0]]);
//	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
//}


////Current Pixel
//vec2 p5 = vec2(inData.uv.x * inData.screenSize.x,inData.uv.y * inData.screenSize.y);
//
////Nighebouring pixel
//vec2 p1 = vec2(p5 + vec2(-1.0,-1.0));
//vec2 p2 = vec2(p5 + vec2(0,-1.0));
//vec2 p3 = vec2(p5 + vec2(1,-1.0));
//vec2 p4 = vec2(p5 + vec2(-1,0));
//vec2 p6 = vec2(p5 + vec2(1.0,0));
//vec2 p7 = vec2(p5 + vec2(-1,1));
//vec2 p8 = vec2(p5 + vec2(0,1));
//vec2 p9 = vec2(p5 + vec2(1.0,1.0));
//
//vec2 pxielUV = vec2(p1.x/inData.screenSize.x, p1.y / inData.screenSize.y);

//	vec4 screen = texture(ScreenGrabTexture, colours[4]);




// float modelScaleX = instance[gl_InstanceID].scale.x;
// float modelScaleY = instance[gl_InstanceID].scale.y;
//
////Set the scale
//   mat4 iModelMat = mat4(
//                     vec4(modelScaleX, 0.0f, 0.0f, 0.0f),
//					  vec4(0.0f,modelScaleY, 0.0f, 0.0f),
//					  vec4(0.0f, 0.0f, 1.0f, 0.0f),
//				  vec4(pos.x, pos.y, pos.z, 1.0f) );
//
//iModelMat *= modelMatrix;

//
//float modelScaleX = instance[gl_InstanceID].scale.x;
//float modelScaleY = instance[gl_InstanceID].scale.y;
//
////Set the scale
//mat4 iModelMat = mat4(
//	vec4(modelScaleX, 0.0f, 0.0f, 0.0f),
//	vec4(0.0f, modelScaleY, 0.0f, 0.0f),
//	vec4(0.0f, 0.0f, 1.0f, 0.0f),
//	vec4(pos.x, pos.y, pos.z, 1.0f));
//
//iModelMat *= modelMatrix;
//vec4 position = iModelMat * pos;



//
//
//
//	//Calculate normals and light directions
//vec3 normal = normalMatrix * vec3(vertexNormal.xyz);
//normal = normalize(normal);
//
////Calucate light direction
//vec3 lightDir = vec3(LightPosition - vec3(position.xyz));
//lightDir = normalize(lightDir);
//
//// //Calculate view Direction
////  vec3 viewPos = viewMatrix[3].xyz;
////  vec3 viewDir = vec3(viewPos - vec3(position.xyz));
////  viewDir = normalize(viewDir);
//// 
////  //Calculate halfway dir
////  vec3 halfwayDir = (lightDir + viewDir);
////  halfwayDir = normalize(halfwayDir);
////  
////  float shininess = 32.00f;
//// 
////  float light =  max(dot(halfwayDir, normal), 0.0);
//// light = pow(light, shininess);
//
//float distance = dot(normal, lightDir);
//
////Set outdata
//outData.intensity = vec4(vec3(distance), 1.0);
////  outData.intensity = vec4(vec3(light),1.0);
// // outData.light = light;
//outData.uv = texCoord;