#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

using namespace std;

class Texture
{
public:
	unsigned int ID;

public:
	Texture(const GLchar* texPath)
	{
		//ID
		unsigned int ID;
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		int width1, height1, nChannels1;
		stbi_set_flip_vertically_on_load(true);	//·­×ªÍ¼Æ¬uv
		unsigned char *data1 = stbi_load(texPath, &width1, &height1, &nChannels1, 0);

		if (data1 != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "Failed to load texture" << endl;
		}

		stbi_image_free(data1);
	}

	~Texture()
	{
	}

private:

};



#endif