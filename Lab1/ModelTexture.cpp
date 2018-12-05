#include "ModelTexture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

ModelTexture::ModelTexture(const std::string& fileName)
{
	//variables for width, height and number of component
	int width, height, numComponents; 
	//load the image from the specified file and store it into the variable
	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4); 

	//return msg if no data found																							 
	if (imageData == NULL)
	{
		std::cerr << "cannot find any texture" << fileName << std::endl;
	}

	//generate texture based on the number and address of textures
	glGenTextures(1, &textureManager); 

	//bind texture with specify type
	glBindTexture(GL_TEXTURE_2D, textureManager); 

	//set up texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	//specify a 2d texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	//free the loaded image
	stbi_image_free(imageData);
}

ModelTexture::~ModelTexture()
{
	glDeleteTextures(1, &textureManager); 
}

void ModelTexture::Bind(unsigned int unit)
{
	//check if the textures are within range, if not terminate the program
	assert(unit >= 0 && unit <= 31); 
	//select active texture unit
	glActiveTexture(GL_TEXTURE0 + unit); 
	//bind texture with specify type
	glBindTexture(GL_TEXTURE_2D, textureManager); 
}
