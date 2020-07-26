#pragma once

#include "Component.hpp"
#include "Core/Log.hpp"
#include <map>
#include <cassert>

class Entity
{
public:
	Entity();
	~Entity();

	void Attach(Component* component);
	void Detach(COMPONENTTYPE componentType);

private:
	std::map<COMPONENTTYPE, Component*> mapComponents;
};

void Entity::Attach(Component* component)
{
	assert(component != nullptr);

	if (mapComponents.find(component->type()) != mapComponents.end())
	{
		Log::log("entity already has the component!");
	}

	component->pEntity = this;
	mapComponents[component->type()] = component;
}

void Entity::Detach(COMPONENTTYPE componentType)
{
	auto component = mapComponents.find(componentType);
	component->second->pEntity = nullptr;
	mapComponents.erase(component);
}


Entity::Entity()
{
}

Entity::~Entity()
{
}