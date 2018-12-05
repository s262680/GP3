#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "MyCamera.h"

struct Transform
{
public:
	
	//calculate the model matrix
	inline glm::mat4 GetModel() const
	{
		glm::mat4 positionMatrix = glm::translate(position);
		glm::mat4 scaleMatrix = glm::scale(scale);
		glm::mat4 rotationX = glm::rotate(rotation.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotationY = glm::rotate(rotation.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotationZ = glm::rotate(rotation.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotationMatrix = rotationX * rotationY * rotationZ;

		return positionMatrix * rotationMatrix * scaleMatrix;
	}


	//getter and setter for model position, rotation and scale
	inline glm::vec3 GetPosition() { return position; } 
	inline glm::vec3 GetRotation() { return rotation; }
	inline glm::vec3 GetScale() { return scale; }
	
	void SetPosition(glm::vec3& position) { this->position = position; }
	void SetRotation(glm::vec3& rotation) { this->rotation = rotation; }
	void SetScale(glm::vec3& scale) { this->scale = scale; }

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};


