#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "WindowDisplay.h" 
#include "MyShader.h"
#include "ModelMesh.h"
#include "ModelTexture.h"
#include "transform.h"
#include "Audio.h"

enum class GameState{PLAY, EXIT};

class MyGame
{
public:
	//constructor and destructor
	MyGame();
	~MyGame();

	void run();
	void MouseRotation(float x);

private:

	//functions
	void input();
	void initGameData();
	void gameLoop();
	void render();
	bool collision(glm::vec3 position1, float radius1, glm::vec3 position2, float radius2);
	void playEffect(unsigned int Source, glm::vec3 pos);
	void playAudio(unsigned int Source);

	//new objects
	WindowDisplay gameWindow;
	GameState _gameState;
	MyCamera FPCamera;
	MyCamera TPCamera;
	ModelMesh ground;
	ModelMesh playerOj;
	ModelMesh moveOj;
	ModelMesh wallOj[57];
	ModelMesh targetOj;
	ModelMesh lightOj;
	Audio audio;

	//variables
	int arrayLength;
	float counter;
	float playerOjAngle;
	bool moveOjWallCollisionTrigger;
	bool playerOjWallCollisionTrigger;
	bool triggerOnce;
	bool tpCameraTrigger;
	bool shaderTrigger;
	string shaderPath;
	bool fogClearColorTrigger;
	unsigned int bg;
	unsigned int friction;
	unsigned int bump;
	unsigned int footstep;
	unsigned int goal;
	float mouseMoveSpeed;

};

