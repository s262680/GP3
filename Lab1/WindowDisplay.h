#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class WindowDisplay
{
public:
	WindowDisplay();
	~WindowDisplay();
	void initDisplay();
	void swapBuffer();
	void clearDisplay(float r, float g, float b, float a);

	//getter of window width and height
	float getWidth() { return screenWidth; } 
	float getHeight() { return screenHeight; }

private:

	//output error message
	void returnError(std::string errorString);
	//hold context
	SDL_GLContext glContext; 
	//hold window pointer 
	SDL_Window* gameWindow; 
	//width and height for the window
	float screenWidth;
	float screenHeight;
};

