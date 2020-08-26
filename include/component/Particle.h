#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../core/Component.h"
#include "../component/Transform.h"

using namespace std;
using namespace glm;

class Particle: public Component
{
public:

	static const ComponentType TYPE = COMPONENT_RENDERER;
	Transform* transform;

	ComponentType type() const
	{
		return TYPE;
	}


	Particle()
	{
		transform = nullptr;
	}

};

#endif // !PARTICLE_H
