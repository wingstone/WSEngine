#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
#include <vector>

#include "../core/Component.h"
#include "../component/Camera.h"
#include "../component/Light.h"
#include "../component/MeshRender.h"
#include "../utils/GeometryGenerator.h"
#include "../WSEngine.h"

using namespace std;
using namespace glm;

class WSEngine;

class RenderManager
{
private:
	LightSetting lightSetting;
	Entity *mainCamera;
	vector<Entity *> lights;
	vector<Entity *> renders;

	Texture *_skybox;
	ShaderClass *skyboxShader;
	Mesh *skyboxMesh;

public:
	WSEngine *engine;

public:
	RenderManager()
	{
		lightSetting.ambientColor = vec4(0.5, 0.5, 0.5, 1.0);
		lightSetting.ambientIntensity = 1.0f;
		mainCamera = nullptr;
		engine = nullptr;

		_skybox = nullptr;
		skyboxShader = nullptr;
		skyboxMesh = nullptr;
	}
	RenderManager(WSEngine *engine)
	{
		lightSetting.ambientColor = vec4(0.5, 0.5, 0.5, 1.0);
		lightSetting.ambientIntensity = 1.0f;
		this->engine = engine;
		mainCamera = nullptr;

		_skybox = nullptr;
		skyboxShader = nullptr;
		skyboxMesh = nullptr;
	}

	void Init(ShaderClass *shader, Mesh* mesh)
	{
		//skybox
		skyboxShader = shader;
		skyboxMesh = mesh;
	}

	void SetAmbientColor(vec4 color)
	{
		lightSetting.ambientColor = color;
	}

	void SetAmbientIntensity(float inten)
	{
		lightSetting.ambientIntensity = inten;
	}

	void SetMainCamera(Entity *camera)
	{
		mainCamera = camera;
	}

	void SetSkyBox(Texture *skybox)
	{
		_skybox = skybox;
	}

	void RemoveMainCamera(Entity *camera)
	{
		if (mainCamera == camera)
		{
			mainCamera = nullptr;
		}
	}

	void AddLight(Entity *light)
	{
		lights.push_back(light);
	}

	void RemoveLight(Entity *light)
	{
		for (auto iter = lights.begin(); iter != lights.end(); iter++)
		{
			if (*iter == light)
			{
				lights.erase(iter);
			}
		}
	}

	void AddMeshRender(Entity *meshRender)
	{
		renders.push_back(meshRender);
	}

	void RemoveMeshRender(Entity *meshRender)
	{
		for (auto iter = renders.begin(); iter != renders.end(); iter++)
		{
			if (*iter == meshRender)
			{
				renders.erase(iter);
			}
		}
	}

	bool Render()
	{
		if (mainCamera == nullptr)
		{
			GlobalLog::Log("no main camera");
			return false;
		}
		if (lights.size() == 0)
		{
			GlobalLog::Log("no light");
			return false;
		}
		for (auto iter : renders)
		{
			Camera *camera = mainCamera->GetComponent<Camera>();
			Light *light = lights[0]->GetComponent<Light>();
			MeshRender *render = iter->GetComponent<MeshRender>();
			render->DrawModel(render->transform, camera, light, &lightSetting);
		}

		//render skybox
		skyboxShader->use();
		Camera *camera = mainCamera->GetComponent<Camera>();
		unsigned int viewLoc = glGetUniformLocation(skyboxShader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));
		unsigned int projectionLoc = glGetUniformLocation(skyboxShader->ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(camera->GetProjectionMatrix()));
		skyboxShader->setTexture("skybox", _skybox->ID, 0, true);
		skyboxMesh->DrawMesh();

		return true;
	}

	void Quit()
	{
	}
};

#endif