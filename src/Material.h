#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Texture.h"
#include "ShaderClass.h"

using namespace std;
using namespace glm;

class Material
{
public:
	vector<Texture*> textures;
	ShaderClass* pshader;
	vec4 diffuseColor;
	vec4 specularColor;
	float smoothness;

public:
	Material()
	{
		this->pshader = nullptr;
	}
	Material(ShaderClass* pshader, vector<Texture*> textures = vector<Texture*>())
	{
		this->textures = textures;
		this->pshader = pshader;
		diffuseColor = vec4(0.9f, 0.6f, 0.8f, 1.0f);
		specularColor = vec4(0.4f, 0.6f, 0.3f, 1.0f);
		smoothness = 0.7f;
	}

	void Use()
	{
		pshader->use();

		//set texture
		unsigned int id = 0;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			pshader->setInt("texture" + to_string(i), i);
			glBindTexture(pshader->ID, textures[i]->ID);
		}

		//set color;
		pshader->setFloat4("diffuseColor", diffuseColor);
		pshader->setFloat4("specularColor", specularColor);
		pshader->setFloat("smoothness", smoothness);
	}

	~Material()
	{
	}

private:

};

#endif