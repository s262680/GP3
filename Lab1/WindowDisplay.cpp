#include "WindowDisplay.h"

//default window values
WindowDisplay::WindowDisplay()
{
	gameWindow = nullptr;  
	screenWidth = 1024.0f;
	screenHeight = 768.0f; 
}

//delete context, window and quit the application at the end
WindowDisplay::~WindowDisplay()
{
	SDL_GL_DeleteContext(glContext); 
	SDL_DestroyWindow(gameWindow); 
	SDL_Quit();
}

//swap buffer
void WindowDisplay::swapBuffer()
{
	SDL_GL_SwapWindow(gameWindow); 
}

//setup background color by passing color value to clear color
void WindowDisplay::clearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void WindowDisplay::initDisplay()
{
	//initial everything
	SDL_Init(SDL_INIT_EVERYTHING);

	//minimize number of bit that used for colour
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	//set up depth buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	//set up double buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//creating window with specificed position and size
	gameWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); 

    //return error message if no window found
	if (gameWindow == nullptr)
	{
		returnError("Failed to create window");
	}

	//create gl context
	glContext = SDL_GL_CreateContext(gameWindow);

	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}

	//return error message if no context found
	GLenum error = glewInit();
	//return error message if glew is unable to initialise
	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}
	//enable depth buffer
	glEnable(GL_DEPTH_TEST); 
	//avoid drawing faces that cannot be seen by camera
	glEnable(GL_CULL_FACE); 

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}

void WindowDisplay::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	cout << "press any key to quit";
	int in;
	std::cin >> in;
	SDL_Quit();
}