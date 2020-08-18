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
#include "../utils/EntityManager.h"

using namespace std;
using namespace glm;

class WSEngine;

class RenderManager
{
private:
	bool cullback;
	bool useDepthTest;
	bool useMSAA;
	unsigned int msaaSample;
	bool useHDR;

	float useVignette;
	float useToneMapping;
	float autoExplosion;

	LightSetting lightSetting;
	Entity *defaultCamera;
	vector<Entity *> cameras;
	vector<Entity *> lights;
	vector<Entity *> renders;

	//clear color
	vec4 _clearColor;

	//skybox
	Texture *skybox;
	ShaderClass *skyboxShader;
	Mesh *skyboxMesh;

	//post copy
	ShaderClass *quadShader;
	Mesh *quadMesh;
	RenderTexture *renderTexture;
	RenderTexture *tempRenderTexture;

	//post bloom
	ShaderClass *preShader;
	ShaderClass *blurHShader;
	ShaderClass *blurVShader;
	ShaderClass *mergeShader;
	RenderTexture *blurRT;
	RenderTexture *tempBlurRT;
	float threshold;

	RenderManager()
	{
		cullback = false;
		useDepthTest = true;
		useMSAA = true;
		msaaSample = 4;
		useHDR = true;

		autoExplosion = 1.0f;
		useVignette = 1.0f;
		useToneMapping = 1.0f;

		lightSetting.ambientColor = vec4(0.5, 0.5, 0.5, 1.0);
		lightSetting.ambientIntensity = 1.0f;
		defaultCamera = nullptr;

		skybox = nullptr;
		skyboxShader = nullptr;
		skyboxMesh = nullptr;

		quadShader = nullptr;
		quadMesh = nullptr;
		renderTexture = nullptr;
		tempRenderTexture = nullptr;

		preShader = nullptr;
		blurHShader = nullptr;
		blurVShader = nullptr;
		mergeShader = nullptr;
		blurRT = nullptr;
		tempBlurRT = nullptr;
		threshold = 1.0f;
	}

	void ReSetRenderTexture()
	{
		unsigned int width = 1280;
		unsigned int height = 720;
		glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->framebuffer);
		if (useHDR && useMSAA)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, renderTexture->textureColorbuffer);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSample, GL_RGB16F, width, height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderTexture->textureColorbuffer, 0);
		}
		else if (useHDR && !useMSAA)
		{
			glBindTexture(GL_TEXTURE_2D, renderTexture->textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //must have
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->textureColorbuffer, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (!useHDR && useMSAA)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, renderTexture->textureColorbuffer);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderTexture->textureColorbuffer, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, renderTexture->textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //must have
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->textureColorbuffer, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ReSetTempRenderTexture()
	{
		unsigned int width = 1280;
		unsigned int height = 720;
		glBindFramebuffer(GL_FRAMEBUFFER, tempRenderTexture->framebuffer);

		if (useHDR)
		{
			glBindTexture(GL_TEXTURE_2D, tempRenderTexture->textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //must have
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempRenderTexture->textureColorbuffer, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, tempRenderTexture->textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //must have
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempRenderTexture->textureColorbuffer, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

public:
	static RenderManager &Instance()
	{
		static RenderManager instance;
		return instance;
	}

	void Quit()
	{
	}

	void Init()
	{
		_clearColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);

		//skybox
		vector<string> skypaths{"../../assets/skybox/right.png",
								"../../assets/skybox/left.png",
								"../../assets/skybox/top.png",
								"../../assets/skybox/bottom.png",
								"../../assets/skybox/front.png",
								"../../assets/skybox/back.png"};
		skybox = ResourcesManager::Instance().LoadCubeTexture(skypaths, "skybox");
		skyboxShader = ResourcesManager::Instance().LoadShader("../../assets/skyboxVert.txt", "../../assets/skyboxFrag.txt", "skybox");
		vector<Vertex> skyboxverts;
		vector<unsigned int> skyboxindes;
		GeometryGenerator::GenerateSkyBox(skyboxverts, skyboxindes);
		skyboxMesh = ResourcesManager::Instance().CreateMesh(skyboxverts, skyboxindes, "skybox");

		//defaultCamera
		defaultCamera = EntityManager::Instance().CreateEntity();
		defaultCamera->AddComponent<Camera>();
		Transform *trans = defaultCamera->GetComponent<Transform>();
		trans->position = glm::vec3(0.0f, 0.0f, 5.0f);
		trans->rotation.y = 180.0f;

		//postprocess copy
		quadShader = ResourcesManager::Instance().LoadShader("../../assets/quadVert.txt", "../../assets/quadFrag.txt", "quad");
		vector<Vertex> quadverts;
		vector<unsigned int> quadindes;
		GeometryGenerator::GenerateQuad(quadverts, quadindes);
		quadMesh = ResourcesManager::Instance().CreateMesh(quadverts, quadindes, "quad");
		renderTexture = ResourcesManager::Instance().CreateRenderTexture(1280, 720, "quad", true, useHDR, useMSAA);
		tempRenderTexture = ResourcesManager::Instance().CreateRenderTexture(1280, 720, "tmpRT", false, useHDR, false);

		//postprocess bloom
		preShader = ResourcesManager::Instance().LoadShader("../../assets/bloomVert.txt", "../../assets/bloomPreFrag.txt", "bloomPre");
		blurHShader = ResourcesManager::Instance().LoadShader("../../assets/bloomVert.txt", "../../assets/bloomHBlurFrag.txt", "bloomHBlur");
		blurVShader = ResourcesManager::Instance().LoadShader("../../assets/bloomVert.txt", "../../assets/bloomVBlurFrag.txt", "bloomVBlur");
		mergeShader = ResourcesManager::Instance().LoadShader("../../assets/bloomVert.txt", "../../assets/bloomMergeFrag.txt", "bloomMerge");
		blurRT = ResourcesManager::Instance().CreateRenderTexture(1280, 720, "blurquad", false, useHDR, false);
		tempBlurRT = ResourcesManager::Instance().CreateRenderTexture(1280, 720, "tmpblurRT", false, useHDR, false);
	}

	void SetClearColor(vec4 color)
	{
		_clearColor = color;
	}

	void SetAmbientColor(vec4 color)
	{
		lightSetting.ambientColor = color;
	}

	void SetAmbientIntensity(float inten)
	{
		lightSetting.ambientIntensity = inten;
	}

	void AddCamera(Entity *camera)
	{
		cameras.push_back(camera);
	}

	void RemoveCamera(Entity *camera)
	{
		for (auto iter = cameras.begin(); iter != cameras.end(); iter++)
		{
			if (*iter == camera)
			{
				cameras.erase(iter);
			}
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

	RenderTexture *GetRenderTexture()
	{
		return tempRenderTexture;
	}

	Entity *GetDefaultCamera()
	{
		return defaultCamera;
	}

	void SetDepthTest(bool value)
	{
		useDepthTest = value;
	}

	void SetHDR(bool value)
	{
		useHDR = value;
		ReSetRenderTexture();
		ReSetTempRenderTexture();
	}

	void SetMSAA(bool value)
	{
		useMSAA = value;
		ReSetRenderTexture();
		ReSetTempRenderTexture();
	}

	void SetCullBack(bool value)
	{
		cullback = value;
	}

	void SetAutoExplosion(float value)
	{
		autoExplosion = value;
	}

	void SetVignette(bool value)
	{
		useVignette = value ? 1.0f : 0.0f;
	}

	void SetToneMapping(bool value)
	{
		useToneMapping = value ? 1.0f : 0.0f;
	}

	void SetThreshold(float value)
	{
		threshold =value;
	}

	bool Render()
	{

		if (lights.size() == 0)
		{
			GlobalLog::Log("no light");
			return false;
		}

		//cullback
		if (cullback)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		//aa
		if (useMSAA)
		{
			glEnable(GL_MULTISAMPLE);
			glfwWindowHint(GLFW_SAMPLES, msaaSample);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}

		//depthtest
		if (useDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		//set framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->framebuffer);
		glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//only render for defaultCamera
		for (auto iter : renders)
		{
			Camera *camera = defaultCamera->GetComponent<Camera>();
			Light *light = lights[0]->GetComponent<Light>();
			MeshRender *render = iter->GetComponent<MeshRender>();
			render->DrawModel(render->transform, camera, light, &lightSetting);
		}

		//render for other Camera
		for (auto camera : cameras)
		{
			Camera *renderCamera = defaultCamera->GetComponent<Camera>();
			if (camera == defaultCamera)
				continue;
			for (auto iter : renders)
			{
				Light *light = lights[0]->GetComponent<Light>();
				MeshRender *render = iter->GetComponent<MeshRender>();
				render->DrawModel(render->transform, renderCamera, light, &lightSetting);
			}
		}

		//render skybox
		skyboxShader->use();
		Camera *camera = defaultCamera->GetComponent<Camera>();
		unsigned int viewLoc = glGetUniformLocation(skyboxShader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));
		unsigned int projectionLoc = glGetUniformLocation(skyboxShader->ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(camera->GetProjectionMatrix()));
		skyboxShader->setTexture("skybox", skybox->ID, 0, true);
		skyboxMesh->DrawMesh();

		//copy framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, renderTexture->framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, tempRenderTexture->framebuffer);
		glBlitFramebuffer(0, 0, 1280, 720, 0, 0, 1280, 720, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		//postprocess
		//bloom pre
		glBindFramebuffer(GL_FRAMEBUFFER, blurRT->framebuffer);
		glDisable(GL_DEPTH_TEST);
		preShader->use();
		preShader->setTexture("screenTexture", tempRenderTexture->textureColorbuffer, 0);
		preShader->setFloat("threshold", threshold);
		quadMesh->DrawMesh();

		//bloom blur1
		glBindFramebuffer(GL_FRAMEBUFFER, tempBlurRT->framebuffer);
		blurHShader->use();
		blurHShader->setTexture("screenTexture", blurRT->textureColorbuffer, 0);
		blurHShader->setFloat2("screenTexture_size", 1280, 720);
		quadMesh->DrawMesh();
		//bloom blur2
		glBindFramebuffer(GL_FRAMEBUFFER, blurRT->framebuffer);
		blurVShader->use();
		blurVShader->setTexture("screenTexture", tempBlurRT->textureColorbuffer, 0);
		blurVShader->setFloat2("screenTexture_size", 1280, 720);
		quadMesh->DrawMesh();

		
		//bloom blur1
		glBindFramebuffer(GL_FRAMEBUFFER, tempBlurRT->framebuffer);
		blurHShader->use();
		blurHShader->setTexture("screenTexture", blurRT->textureColorbuffer, 0);
		blurHShader->setFloat2("screenTexture_size", 1280/2, 720/2);
		quadMesh->DrawMesh();
		//bloom blur2
		glBindFramebuffer(GL_FRAMEBUFFER, blurRT->framebuffer);
		blurVShader->use();
		blurVShader->setTexture("screenTexture", tempBlurRT->textureColorbuffer, 0);
		blurVShader->setFloat2("screenTexture_size", 1280/2, 720/2);
		quadMesh->DrawMesh();

		
		//bloom blur1
		glBindFramebuffer(GL_FRAMEBUFFER, tempBlurRT->framebuffer);
		blurHShader->use();
		blurHShader->setTexture("screenTexture", blurRT->textureColorbuffer, 0);
		blurHShader->setFloat2("screenTexture_size", 1280/4, 720/4);
		quadMesh->DrawMesh();
		//bloom blur2
		glBindFramebuffer(GL_FRAMEBUFFER, blurRT->framebuffer);
		blurVShader->use();
		blurVShader->setTexture("screenTexture", tempBlurRT->textureColorbuffer, 0);
		blurVShader->setFloat2("screenTexture_size", 1280*4, 720*4);
		quadMesh->DrawMesh();

		//bloom merge
		glBindFramebuffer(GL_FRAMEBUFFER, tempRenderTexture->framebuffer);
		mergeShader->use();
		mergeShader->setTexture("screenTexture", tempRenderTexture->textureColorbuffer, 0);
		mergeShader->setTexture("blurScreenTexture", blurRT->textureColorbuffer, 1);
		quadMesh->DrawMesh();

		//copy
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quadShader->use();
		quadShader->setTexture("screenTexture", tempRenderTexture->textureColorbuffer, 0);
		quadShader->setFloat("autoExplosion", autoExplosion);
		quadShader->setFloat("useToneMapping", useToneMapping);
		quadShader->setFloat("useVignette", useVignette);
		quadMesh->DrawMesh();

		return true;
	}
};

#endif