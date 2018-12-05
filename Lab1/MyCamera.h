#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct MyCamera
{
public:

	//initial the camera by taking position, field of view, aspect ratio and near / far clipping values
	void initCamera(const glm::vec3& position, float fov, float aspect, float near, float far)
	{
		//set the variables equal to the arguments
		this->position = position;
		this->zDir = glm::vec3(0.0f, 0.0f, 1.0f);
		this->yDir = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, near, far);
	}

	//getter of camera view projection matrix
	inline glm::mat4 GetCamViewProjection() const
	{
		return projection * glm::lookAt(position, position + zDir, yDir);
	}

	//getter of camera view matrix
	inline glm::mat4 GetCamViewMatrix() const
	{
		return glm::lookAt(position, position + zDir, yDir);
	}

	//function that allows camera to move forward
	void CamForwardMovement(float rate)
	{
		position += zDir * rate;
	}

	//function that allows camera to rotate vertically
	void VerticalRotation(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(yDir, zDir));

		zDir = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(zDir, 0.0)));
		yDir = glm::normalize(glm::cross(zDir, right));
	}

	//function that allows camera to rotate horizontally
	void HorizonRotation(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		zDir = glm::vec3(glm::normalize(rotation * glm::vec4(zDir, 0.0)));
		yDir = glm::vec3(glm::normalize(rotation * glm::vec4(yDir, 0.0)));
	}


	//getter of the forward z direction of the camera
	glm::vec3 getForward()
	{
		return this->zDir;
	}

	//getter of camera position
	inline glm::vec3 GetCameraPosition() 
	{ 
		return position; 
	}

	//setter of camera position
	void setCameraPos(glm::vec3 position)
	{
		this->position = position;
	}

protected:
private:
	//private variable for this class
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 zDir;
	glm::vec3 yDir;
};


