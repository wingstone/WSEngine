#pragma once

#include <string>

enum COMPONENTTYPE
{
	COMPONENT_NONE,
	COMPONENT_TRANSFORM,
	COMPONENT_CAMERA,
	COMPONENT_RENDER,
	COMPONENT_LIGHT,
	COMPONENT_CONTROLLER,
};

class Entity;

class Component
{
public:
	friend class Entity;

	Component();
	virtual ~Component();

	virtual COMPONENTTYPE type() const = 0;

protected:
	Entity* pEntity;
	COMPONENTTYPE componentType;
};

Component::Component()
{
	pEntity = nullptr;
	componentType = COMPONENT_NONE;
}

Component::~Component()
{
}