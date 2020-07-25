#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace glm;

struct Light
{
	vec4 lightColor;
	vec4 lightPos;		//w = 0表示方向光，w = 1表示点光

	Light()
	{
		lightColor = vec4(1.0f);
		lightPos = vec4(0.707f, 0.707f, 0.0f, 0.0f);
	}
};

struct AmbientLight
{
	vec4 ambientColor;
	float ambientIntensity;

	AmbientLight()
	{
		ambientColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		ambientIntensity = 1.0f;
	}
};

#endif // !SHADER_H
