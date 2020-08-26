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

	enum LightType{
		DIRECTIONAL,
		POINT,
		//SPOT
	};

	static const ComponentType TYPE = COMPONENT_LIGHT;
	Transform* transform;
	vec4 lightColor;
	LightType lighttype;
	float lightIntensity;

	ComponentType type() const
	{
		return TYPE;
	}


	Light()
	{
		transform = nullptr;
		lightColor = vec4(1.0f);
		lightIntensity = 1;
		lighttype = DIRECTIONAL;
	}

	mat4 GetShadowMatrix()
	{
		vec3 position = transform->position;
		mat4 view = lookAtRH(position, position + transform->Front(), transform->Up());
		mat4 proj = ortho(-4.0f, 4.0f,-4.0f, 4.0f, -3.0f, 3.0f);
		return proj*view;
	}
};

#endif // !SHADER_H
