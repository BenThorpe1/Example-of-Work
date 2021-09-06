// NOTE> If you get a bunch of unresolved external symbols errors relating to server/client code, 
// then open up your Properties window and check you have the following libraries included in your 
// project: Linker -> Input -> Additional Dependencies: wsock32.lib;Ws2_32.lib;

//Includes
#include "stdafx.h"

cNetworkManager networkManager;

bool isQuit = false;

using namespace glm;
// global variables
Scene scene;
bool bHideCursor = false;
//Whether sever or client
int CurrentMode = 0;

//Callback when resized
void resize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window
}

// Callback when mouse moves, Player object moves with mouse
static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	int hieght;
	int wiegth;
	glfwGetWindowSize(window, &wiegth, &hieght);

	//Calculate ypos on screen
	float sy = yPos / hieght;
	float y = ((1.0f - sy) * 2.0f) - 1;

	float sx = xPos / wiegth;
	float x = ((1.0f - sx) * 2.0f) - 1;
	scene.ReciveMouseInput(vec2(x, y));
}

// Contorls all keyboard interactions
static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// ignore key up (key release) events
	if (action == GLFW_RELEASE)
	{
		return;
	}
	// process keyboard inputs here..
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
	case GLFW_KEY_X:
		glfwSetWindowShouldClose(window, true);
		break;

	case GLFW_KEY_H:
	{
		if (bHideCursor == false)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			bHideCursor = true;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			bHideCursor = false;
		}
	}

	default:
	{
	}
	}

	scene.ReciveKeyBoardInput(key);
}

/////////////////////////////////////////////////////////////////////////////////////
// printDebugInfo() - print version info to console
/////////////////////////////////////////////////////////////////////////////////////
void printDebugInfo()
{
	// Print info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

#ifdef GL_SHADING_LANGUAGE_VERSION
	printf("Supported GLSL version is %s.\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

	printf("Using GLEW version %s.\n", glewGetString(GLEW_VERSION));
	printf("------------------------------\n");
	printf("Press ESCAPE or 'X' or 'x' to exit.\n");
}

// update() - update scene
void Update(float a_deltaTime)
{
	scene.Update(a_deltaTime);
}

// comment this out if you want to run seperate client / server instances
//#define DEBUG

//////////////////////////////////////////////////////////////////////////////////////////////
// main() - pass params using argv[] i.e. 0 for server, 1 for client
//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	bool isSever = false;
	//Set Network
	const char* ipAddress = argv[2];
	if (ipAddress == nullptr)
	{
		ipAddress = "localHost";
	}


#ifdef DEBUG
	ipAddress = "localHost";
	isSever = true;
	networkManager.Add(true,ipAddress, &scene);
	networkManager.Add(false, ipAddress, &scene);

#else
	// NOTE: Open up two comand prompt windows.
	// Change the directory (cd) to the Debug directory where this 
	// executable can be found. Enter the name of the .exe along
	// with the required arguments as follows:
	// To run a SERVER instance, enter example.exe 0
	// To run a CLIENT instance, enter example.exe 1
	if (*argv[1] == '0')
	{
		// SERVER
		isSever = true;
		networkManager.Add(true,ipAddress,&scene);
	}
	else
	if(*argv[1] == '1')
	{
		isSever = false;
		// CLIENT
		networkManager.Add(false, ipAddress,&scene);
	}
	else
	{
		std::cout << "Error\n";
	}
#endif

	//Set up GLf
	char windowTitle[] = "GraphicsTemplate";

	// initialise glfw
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, windowTitle, NULL, NULL);
	if (window == NULL)
	{
		fprintf(stdout, "Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialise glew
	glewInit();

	// print version info 
	printDebugInfo();

	// Set callback function for resizing the window
	glfwSetFramebufferSizeCallback(window, resize);


	// Set callback for keyboard events
	glfwSetKeyCallback(window, keyboard);

	glfwSetCursorPosCallback(window, cursorPosCallback);

	resize(window, initWidth, initHeight);

	check_for_opengl_errors();

	scene.Init(isSever);
	float lastTime = glfwGetTime();
	float curTime = 0;

	if (isSever)
	{
		scene.SetSever(&networkManager.m_server);
	}
	else
	{
		scene.SetClient(&networkManager.m_client);
	}

	while (isQuit == false)
	{
	
		//Upadte
		curTime = glfwGetTime();
		Update(curTime - lastTime);
		lastTime = curTime;
		//Network
		networkManager.Update();
		//Render
		scene.RenderScene();				// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		// Poll events (key presses, mouse events)
//		glfwWaitEvents();					// Use this if no animation.
		//glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();
		isQuit = glfwWindowShouldClose(window);
	}
	scene.DeInit();
	networkManager.Deinit();
	glfwTerminate();
	return 1;
}


