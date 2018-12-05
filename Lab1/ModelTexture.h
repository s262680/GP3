#pragma once
#include <string>
#include <GL\glew.h>

class ModelTexture
{
public:
	

	//bind texture up to a specified number which will be the unit variable
	void Bind(unsigned int unit);
	//load texture from a specified file path
	ModelTexture(const std::string& fileName);
	//destructor
	~ModelTexture();
private:

	GLuint textureManager;
};

