#pragma once

#include <glm/glm.hpp>

using namespace glm;

struct LightSetting
{
	vec4 ambientColor;
	float ambientIntensity;

	LightSetting()
	{
		ambientColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		ambientIntensity = 1.0f;
	}
};
