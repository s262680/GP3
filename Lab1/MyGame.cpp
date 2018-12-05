#include "MyGame.h"
#include "MyCamera.h"
#include <iostream>
#include <string>
#include <Windows.h>

Transform transform;
Transform transform1;
Transform transform2;

//resource folder path
const string resPath = "..\\res\\";

//states of key press
const Uint8* keystates = SDL_GetKeyboardState(NULL);
//window
HWND hwnd;
//rectangle of window
RECT r;
//cursor position
POINT point;

//constructor
MyGame::MyGame()
{
	//game will run when game state equal 0, close when equal 1
	_gameState = GameState::PLAY;
	//create new window
	WindowDisplay* gameWindow = new WindowDisplay(); 

}

MyGame::~MyGame()
{
}

//call by main class to run initGameData() and start main game loop
void MyGame::run()
{
	initGameData(); 
	gameLoop();

}


//setup source and value for several objects
void MyGame::initGameData()
{
	//initial game screen
	gameWindow.initDisplay();
	
	//set up audio source
	bg = audio.loadSound("..\\res\\bg2.wav");
	friction = audio.loadSound("..\\res\\heavyOj.wav");
	bump= audio.loadSound("..\\res\\bump2.wav");
	footstep= audio.loadSound("..\\res\\footstep.wav");
	goal= audio.loadSound("..\\res\\goal.wav");

	//Get the length of the array
	arrayLength = sizeof(wallOj) / sizeof(wallOj[0]); 
	//setup model source
	ground.loadGameModel("..\\res\\basketball.obj");
	playerOj.loadGameModel("..\\res\\legoman.obj");
	moveOj.loadGameModel("..\\res\\tree.obj");
	for (int i = 0; i < arrayLength; i++)
	{
		wallOj[i].loadGameModel("..\\res\\tree.obj");
	}
	targetOj.loadGameModel("..\\res\\tree.obj");
	lightOj.loadGameModel("..\\res\\tree.obj");


	//initial camera with position, fov, aspect ratio, near and far clipping
	FPCamera.initCamera(glm::vec3(15, 0, -15), 70.0f, (float)gameWindow.getWidth()/ gameWindow.getHeight(), 0.01f, 1000.0f);
	TPCamera.initCamera(glm::vec3(0, 30, -22), 70.0f, (float)gameWindow.getWidth() / gameWindow.getHeight(), 0.01f, 1000.0f);
	//look down angle for top down second camera
	TPCamera.VerticalRotation(45);

	//trigger for two types of collisions
	moveOjWallCollisionTrigger = false;
	playerOjWallCollisionTrigger = false;
	//trigger for initial object position
	triggerOnce = false;
	
	//trigger for switching camera
	tpCameraTrigger = false;
	//trigger for changing fog clear color
	fogClearColorTrigger = false;
	//player object's y rotation
	playerOjAngle = 0;

	//default shader path
	shaderPath = resPath + "fog";

	//mouse rotation control speed
	mouseMoveSpeed = 0.005f;
	
	//hide cursor
	ShowCursor(false);

	//lock cursor inside the window
	hwnd = FindWindow(0, "Game Window");	
	GetWindowRect(hwnd, &r);
	ClipCursor(&r);

}


//the main game loop 
void MyGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		input();
		render();
		playAudio(bg);
		//update the position of the cursor
		GetCursorPos(&point);

		//check each wall object
			for (int i = 0; i < arrayLength; i++)
			{
				if (!playerOjWallCollisionTrigger && !moveOjWallCollisionTrigger)
				{
					//check collision between player and wall 
					if (collision(playerOj.getSpherePosition(), playerOj.getSphereRadius(), wallOj[i].getSpherePosition(), wallOj[i].getSphereRadius()))
					{
						playerOjWallCollisionTrigger = true;
					}
					else
					{
						playerOjWallCollisionTrigger = false;
					}

					//check collision between moveable object and wall 
					if (collision(moveOj.getSpherePosition(), moveOj.getSphereRadius(), wallOj[i].getSpherePosition(), wallOj[i].getSphereRadius()))
					{
						moveOjWallCollisionTrigger = true;
					}
					else
					{
						moveOjWallCollisionTrigger = false;
					}
				}
			}
		
			//trigger for clear color if using fog shader
			if (shaderPath == resPath + "fog")
			{
				fogClearColorTrigger = true;
			}
			else
			{
				fogClearColorTrigger = false;
			}

			
	}
}

//Sphere collision detection function
bool MyGame::collision(glm::vec3 position1, float radius1, glm::vec3 position2, float radius2)
{
	//calculate distance between two objects
	float x = (position2.x - position1.x)*(position2.x - position1.x);
	float y = (position2.y - position1.y)*(position2.y - position1.y);
	float z = (position2.z - position1.z)*(position2.z - position1.z);
	float distance = glm::sqrt(x + y + z);
	
	//check if the distance is less than the specified radius, objects are collided if true, otherwise false
	if (distance < (radius1 + radius2))
	{	
		return true;
	}
	else
	{
		return false;
	}
}



//function that takes several types of input and result
//moving or rotating camera or objects
void MyGame::input()
{
	SDL_Event event;

	//while there is events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{		
			//pass the mouse motion value to the mouse rotation function
		case SDL_MOUSEMOTION:

			//rotation only happen when cursor stay within the window
			if (point.x > r.left+10 && point.x < r.right-10 && point.y<r.bottom-10 && point.y>r.top+10)
			{
				MouseRotation(event.motion.xrel);
			}
			else
			{			
				//set the cursor back to the centre of the window if it touch the edge of the window
				SetCursorPos((r.left+r.right) / 2, (r.top+r.bottom) / 2);
			}
			break;
			
		//close game when event type equal to quit
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;


		case SDL_KEYDOWN:
			//create a keytype variable to represent the keycode
			SDL_Keycode keyType = event.key.keysym.sym;

			//perform different action depending on the key type
			switch (keyType)
			{
			
			//set game state to 1 when pressing p to close the game
			case SDLK_p:
				_gameState = GameState::EXIT;
				break;

			//switch between two camera when pressing q
			case SDLK_q:
				if (!tpCameraTrigger)
				{
					tpCameraTrigger = true;
				}
				else
				{
					tpCameraTrigger = false;
				}
				break;
		
				//pressing 1 to switch to basic shader without any effect
				case SDLK_1:			
					shaderPath = resPath + "shader";
					break;
				//pressing 2 to switch to shader with lighting effect
				case SDLK_2:
					shaderPath = resPath + "simpleLight";
					break;
				//pressing 3 to switch to shader with fog effect
				case SDLK_3:
					shaderPath = resPath + "fog";
					break;

				//for testing only
				case SDLK_4:
					shaderPath = resPath + "blur";
					break;
				case SDLK_5:
					shaderPath = resPath + "test";
					break;
				}
			
		}
	
	}


	//different key states, w and s to move forward and backward and play foot step audio
	if (keystates[SDL_SCANCODE_W])
	{
		FPCamera.CamForwardMovement(0.2f);
		playAudio(footstep);
	}
	else if (keystates[SDL_SCANCODE_S])
	{
		FPCamera.CamForwardMovement(-0.2f);
		playAudio(footstep);
	}

	//a and d as an alternative way to rotate camera and player object horizontally
	if (keystates[SDL_SCANCODE_A])
	{
		FPCamera.HorizonRotation(0.2f);
		playerOjAngle += 0.2f;
	}
	else if (keystates[SDL_SCANCODE_D])
	{
		FPCamera.HorizonRotation(-0.2f);
		playerOjAngle -= 0.2f;
	}
}


//function that allows camera and player object rotation by using mouse motion
void MyGame::MouseRotation(float x)
{
		FPCamera.HorizonRotation(-x*mouseMoveSpeed);
		playerOjAngle += -x*mouseMoveSpeed;
}



//audio function without the use of object position
void MyGame::playAudio(unsigned int Source)
{
	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);

	if (AL_PLAYING != state)
	{
		audio.playSound(Source);
	}
}

//3d audio function that make use of object position
void MyGame::playEffect(unsigned int Source, glm::vec3 pos)
{
	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);

	if (AL_PLAYING != state)
	{
		audio.playSound(Source, pos);
	}
}




//rendering function
void MyGame::render()
{
	//setup clear color depend on the trigger state
	if (fogClearColorTrigger)
	{
		gameWindow.clearDisplay(0.6f, 0.6f, 0.6f, 1.0f);
	}
	else
	{
		gameWindow.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	}

	//create new shader
	MyShader shader(shaderPath);
	shader.Bind();

	//setup textures
	ModelTexture wall("..\\res\\wall.jpg");
	ModelTexture crystal("..\\res\\crystal.jpg");
	ModelTexture floor("..\\res\\floor.jpg");
	ModelTexture yellowbox("..\\res\\yellowbox.png");



//setup and render the ground object##################################

	//set position, rotation and scale of the object
	transform.SetPosition(glm::vec3(1.0, -1.0, 1.0));
	transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(2, 0.01, 2));

	

	//passing different camera to shader depending on the trigger
	if (!tpCameraTrigger)
	{
		shader.Update(transform, FPCamera);
	}
	else
	{
		shader.Update(transform, TPCamera);
	}

	//bind texture
	floor.Bind(0);
	//draw model
	ground.drawModel();



//setup and render the goal object####################################

	transform.SetPosition(glm::vec3(-15.0, -1.0, 15.0));
	transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(0.1, 5, 0.1));

	
	if (!tpCameraTrigger)
	{
		shader.Update(transform, FPCamera);
	}
	else
	{
		shader.Update(transform, TPCamera);
	}
	yellowbox.Bind(0);
	targetOj.drawModel();
	//passing object position and sphere radius for collision detection check
	targetOj.setupSphere(transform.GetPosition(), 0.2f);



//setup and render the wall objects######################################

	//top walls
	for (int i = 0; i < 7; i++)
	{
		transform.SetPosition(glm::vec3(20.0-i*6, -1.0, 20.0));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(2, 2, 2));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 3.0f);
	}

	//left walls
	for (int i = 7; i < 14; i++)
	{
		transform.SetPosition(glm::vec3(20, -1.0, 20.0 - (i - 7) * 6));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(2, 2, 2));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 3.0f);
	}

	//right walls
	for (int i = 14; i < 21; i++)
	{
		transform.SetPosition(glm::vec3(-20, -1.0, 20.0 - (i - 14) * 6));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(2, 2, 2));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 3.0f);
	}

	//bottom walls
	for (int i = 21; i < 28; i++)
	{
		transform.SetPosition(glm::vec3(20.0 - (i-21) * 6, -1.0, -20.0));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(2, 2, 2));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 3.0f);
	}

	//other inner walls
	for (int i = 28; i < 31; i++)
	{
		transform.SetPosition(glm::vec3(16.0 - (i - 28) * 3, -1.0, 5.0));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}

	for (int i = 31; i < 34; i++)
	{
		transform.SetPosition(glm::vec3(8.0  , -1.0, -16.0 + (i - 31)*3));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}

	for (int i = 34; i < 39; i++)
	{
		transform.SetPosition(glm::vec3(12.0 - (i - 34) * 3, -1.0, -2.0));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}

	for (int i = 39; i < 42; i++)
	{
		transform.SetPosition(glm::vec3(-6.0 , -1.0, 1.0 - (i - 39) * 3));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}

	for (int i = 42; i < 44; i++)
	{
		transform.SetPosition(glm::vec3(-4.0 + (i - 42) * 3, -1.0, 4.0 ));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}

	for (int i = 44; i < 49; i++)
	{
		transform.SetPosition(glm::vec3(0 - (i - 44) * 3, -1.0, -12));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}

	for (int i = 49; i < 54; i++)
	{
		transform.SetPosition(glm::vec3(-12.0, -1.0, 7.0 - (i - 49) * 3));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}

	for (int i = 54; i < 57; i++)
	{
		transform.SetPosition(glm::vec3(5.0, -1.0, 17.0 - (i - 54) * 3));
		transform.SetRotation(glm::vec3(0.0, 0.0, 0.0));
		transform.SetScale(glm::vec3(1, 1, 1));
		
		if (!tpCameraTrigger)
		{
			shader.Update(transform, FPCamera);
		}
		else
		{
			shader.Update(transform, TPCamera);
		}
		wall.Bind(0);
		wallOj[i].drawModel();
		wallOj[i].setupSphere(transform.GetPosition(), 1.5f);
	}


//setup and render the player object###############################################

	if (moveOjWallCollisionTrigger||playerOjWallCollisionTrigger)
	{
		//push player back if moving forward and collided with wall, push forward if moving back and collided with wall
		if (keystates[SDL_SCANCODE_S])
		{
			transform1.SetPosition(glm::vec3(transform1.GetPosition().x + (FPCamera.getForward().x*1.1f), transform1.GetPosition().y, transform1.GetPosition().z + (FPCamera.getForward().z*1.1f)));
		}
		else
		{
			transform1.SetPosition(glm::vec3(transform1.GetPosition().x + (FPCamera.getForward().x*-1.1f), transform1.GetPosition().y, transform1.GetPosition().z + (FPCamera.getForward().z*-1.1f)));
		}
		
		//camera follow the position of the object 
		FPCamera.setCameraPos(transform1.GetPosition());
		//play collision sound
		playEffect(bump, transform1.GetPosition());
		playerOjWallCollisionTrigger = false;
	}
	else
	{
		//player object follow the position of camera
		transform1.SetPosition(FPCamera.GetCameraPosition());		
	}

	transform1.SetRotation(glm::vec3(0.0, playerOjAngle, 0.0));
	transform1.SetScale(glm::vec3(0.3, 0.3, 0.3));

	//only render player object when in top down camera
	if(tpCameraTrigger)
	{
		shader.Update(transform1, TPCamera);
		yellowbox.Bind(0);
		playerOj.drawModel();
	}
	
	//passing object position and sphere radius for collision detection check
	playerOj.setupSphere(transform1.GetPosition(), 1.0f);
	



//setup and render the moveable object################################################

	if (!triggerOnce)
	{
		//initial object position
		transform2.SetPosition(glm::vec3(12, -1.0, -12));
		triggerOnce = true;
	}

	//push object back and play sound when collided with a wall
	if (moveOjWallCollisionTrigger)
	{
		transform2.SetPosition(glm::vec3(transform2.GetPosition().x + (FPCamera.getForward().x*-1.1f), transform2.GetPosition().y, transform2.GetPosition().z + (FPCamera.getForward().z*-1.1f)));
		moveOjWallCollisionTrigger = false;
		playerOjWallCollisionTrigger = false;
		playEffect(bump, transform2.GetPosition());
	}
	else
	{
		//push forward when collided with player object and play sound
		if (collision(playerOj.getSpherePosition(), playerOj.getSphereRadius(), moveOj.getSpherePosition(), moveOj.getSphereRadius()))
		{
			//the forward direction are based on the camera looking direction
			transform2.SetPosition(glm::vec3(transform2.GetPosition().x + (FPCamera.getForward().x*1.1f), transform2.GetPosition().y, transform2.GetPosition().z + (FPCamera.getForward().z*1.1f)));
			playEffect(friction,transform2.GetPosition());
		}
		else
		{
			//otherwise not moving
			transform2.SetPosition(transform2.GetPosition());
		}
	}
	
	//play sound and move the object to another location if it collided with the goal object
	if (collision(targetOj.getSpherePosition(), targetOj.getSphereRadius(), moveOj.getSpherePosition(), moveOj.getSphereRadius()))
	{
		playEffect(goal, targetOj.getSpherePosition());
		transform2.SetPosition(glm::vec3(12, -1.0, 12));
	}

	transform2.SetRotation(glm::vec3(0.0, 0.0, 0.0));
	transform2.SetScale(glm::vec3(0.7, 0.7,0.7));

	if (!tpCameraTrigger)
	{
		shader.Update(transform2, FPCamera);
	}
	else
	{
		shader.Update(transform2, TPCamera);
	}
	crystal.Bind(0);
	moveOj.drawModel();
	moveOj.setupSphere(transform2.GetPosition(), 1.3f);


	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	gameWindow.swapBuffer();
} 

