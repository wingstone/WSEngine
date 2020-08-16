#ifndef ENTITY_H
#define ENTITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <map>

#include "Component.h"
#include "../component/Transform.h"

using namespace std;

class EntityManager;

class Entity
{
public:
	template <class Type>
	Type *AddComponent()
	{
		ComponentType t = Type::TYPE;
		auto c = components.find(t);

		if (c != components.end())
		{
			GlobalLog::Log("can't add component ");
			return nullptr;
		}

		Type *component = new Type();
		component->entity = this;
		if (t != COMPONENT_TRANSFORM)
		{
			component->transform = dynamic_cast<Transform *>(components[COMPONENT_TRANSFORM]);
		}
		components[component->type()] = component;

		switch (t)
		{
		case COMPONENT_CAMERA:
			RenderManager::Instance().AddCamera(this);
			break;
		case COMPONENT_LIGHT:
			RenderManager::Instance().AddLight(this);
			break;
		case COMPONENT_RENDERER:
			RenderManager::Instance().AddMeshRender(this);
			break;
		default:
			break;
		}

		return component;
	}

	template <class Type>
	void RemoveComponent(Type *component)
	{
		ComponentType t = component->type();
		auto c = components.find(t);

		if (c != components.end() && c->second == component)
		{
			switch (t)
			{
			case COMPONENT_CAMERA:
				entityManager->engine->renderManager.RemoveMainCamera(this);
				break;
			case COMPONENT_LIGHT:
				entityManager->engine->renderManager.RemoveLight(this);
				break;
			case COMPONENT_RENDERER:
				entityManager->engine->renderManager.RemoveMeshRender(this);
				break;
			default:
				break;
			}
			components.erase(c);
			delete c->second;
		}
		else
		{
			GlobalLog::Log("can't remove component ");
		}
	}

	template <class Type>
	Type *GetComponent()
	{
		ComponentType t = Type::TYPE;
		auto c = components.find(t);

		if (c == components.end())
			return nullptr;

		return dynamic_cast<Type *>(c->second);
	}

	void Destroy()
	{
		for (auto iter = components.begin(); iter != components.end(); iter++)
		{
			components.erase(iter);
			delete iter->second;
		}
	}

	void SetManager(EntityManager *entityManager)
	{
		this->entityManager = entityManager;
	}

private:
	std::map<ComponentType, Component *> components;
	EntityManager *entityManager;
};

#endif