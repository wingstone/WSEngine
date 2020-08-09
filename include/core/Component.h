#ifndef COMPONENT_H
#define COMPONENT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <string>
#include <vector>

class Entity;

using namespace std;

enum ComponentType {
	COMPONENT_TRANSFORM,
	COMPONENT_CAMERA,
	COMPONENT_RENDERER,
	COMPONENT_LIGHT,
};

class Component {
public:
	Entity* entity;

	virtual ComponentType type() const = 0;

};

#endif