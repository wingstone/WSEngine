#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <list>

#include "../core/Entity.h"
#include "../component/Transform.h"

using namespace glm;

class WSEngine;
class Entity;

class EntityManager
{
private:
    std::list<Entity *> entities;
    EntityManager()
    {
    }

public:
	static EntityManager& Instance()
	{
		static EntityManager instance;
		return instance;

	}

    void Init()
    {
    }

    void Quit()
    {
        for (auto iter = entities.begin(); iter != entities.end(); iter++)
        {
            (*iter)->Destroy();
            entities.erase(iter);
            delete *iter;
        }
    }

    Entity *CreateEntity(vec3 position = vec3(0, 0, 0), vec3 rotation = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1))
    {
        Entity *entity = new Entity();
        Transform *trans = entity->AddComponent<Transform>();
        trans->position = position;
        trans->rotation = rotation;
        trans->scale = scale;
        entities.push_back(entity);
        entity->SetManager(this);
        return entity;
    }

    void RemoveEntity(Entity *entity)
    {
        for (auto iter = entities.begin(); iter != entities.end(); iter++)
        {
            if (*iter == entity)
            {
                entities.erase(iter);
                delete *iter;
            }
        }
    }

};

#endif