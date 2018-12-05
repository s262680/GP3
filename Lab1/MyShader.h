#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "MyCamera.h"
#include "MyGame.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
class MyShader
{
public:
	//constructor and destructor
	MyShader(const std::string& filename);
	~MyShader();

	void Bind(); 
	void Update(const Transform& transform, MyCamera& myCamera);
	std::string MyShader::LoadShader(const std::string& fileName);
	void MyShader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint MyShader::CreateShader(const std::string& text, unsigned int type);

  


protected:
private:
	//set the numbers of shaders
	static const unsigned int NUM_SHADERS = 2;
	//a variable that use for store the shader program as reference number
	GLuint program; 
	//array of shaders
	GLuint shaders[NUM_SHADERS];

	//uniforms
	GLint uniMVP;
	GLint uniModel;
	GLint uniViewSpace;
	GLint uniCameraPos;

	
};
