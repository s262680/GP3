#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"



struct Sphere
{
public:

	//getter for the sphere
	glm::vec3 GetPosition() 
	{ 
		return position; 
	}
	float GetRadius() 
	{ 
		return radius; 
	}

	//setter for the sphere
	void SetPosition(glm::vec3 position)
	{
		this->position = position;
	}
	void SetRadius(float radius)
	{
		this->radius = radius;
	}

private:
	glm::vec3 position;
	float radius;
};

class ModelMesh
{
public:

	//constructor and destructor
	ModelMesh();
	~ModelMesh();

	void drawModel();
	void loadGameModel(const std::string& filename);
	void initGameModel(const IndexedModel& model);

	//sphere setter that takes position and radius as parameter 
	void setupSphere(glm::vec3 position, float radius);

	//sphere getter that use by other class
	glm::vec3 getSpherePosition() 
	{ 
		return meshSphere.GetPosition(); 
	}
	float getSphereRadius() 
	{ 
		return meshSphere.GetRadius(); 
	}

private:


	//a pack of numbers that start from 0 and add 1 to the next
	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	Sphere meshSphere;

	//vertex array object and buffer
	GLuint VAO;
	GLuint VAB[NUM_BUFFERS]; 
	//the number of VAO
	unsigned int drawCount; 
};