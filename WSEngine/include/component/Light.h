#ifndef LIGHT_H
#define LIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../core/Component.h"

using namespace std;
using namespace glm;

class Light: public Component
{
public:
	static const ComponentType TYPE = COMPONENT_LIGHT;
	Transform* transform;
	vec4 lightColor;
	vec4 lightPos;		//w = 0表示方向光，w = 1表示点光

	ComponentType type() const
	{
		return TYPE;
	}


	Light()
	{
		transform = nullptr;
		lightColor = vec4(1.0f);
		lightPos = vec4(0.707f, 0.707f, 0.0f, 0.0f);
	}
};

#endif // !SHADER_H
