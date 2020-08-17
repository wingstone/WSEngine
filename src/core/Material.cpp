#include <core/Material.h>

void Material::ImportRenderSetting(Transform* transform, Camera* camera, Light* light, LightSetting* lightSetting)
{
	pshader->use();

	//matrix
	unsigned int modelLoc = glGetUniformLocation(pshader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(transform->GetModelMatrix()));
	unsigned int viewLoc = glGetUniformLocation(pshader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));
	unsigned int projectionLoc = glGetUniformLocation(pshader->ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(camera->GetProjectionMatrix()));

	//light
	pshader->setFloat4("cameraPos", vec4(camera->entity->GetComponent<Transform>()->position, 0.0f));

	pshader->setFloat4("lightPos", light->lightPos);
	pshader->setFloat4("lightColor", light->lightColor);
	pshader->setFloat("lightIntensity", light->lightIntensity);

	pshader->setFloat4("ambientColor", lightSetting->ambientColor);
	pshader->setFloat("ambientIntensity", lightSetting->ambientIntensity);
}

PBRMaterial::PBRMaterial(ShaderClass* pshader): Material(pshader)
{
	this->textures = vector<Texture*>();
	this->strings = vector<string>();
	strings.push_back("albedoTex");
	strings.push_back("metalicTex");
	strings.push_back("normalTex");
	diffuseColor = vec4(0.9f, 0.6f, 0.8f, 1.0f);
	specularColor = vec4(0.4f, 0.6f, 0.3f, 1.0f);
	roughness = 0.7f;
}

void PBRMaterial::Render()
{
	pshader->use();
	//set texture
	unsigned int id = 0;
	for (unsigned int i = 0; i <glm::min(textures.size(), strings.size()) ; i++)
	{
		pshader->setTexture(strings[i], textures[i]->ID, i);
	}

	//set color;
	pshader->setFloat4("diffuseColor", diffuseColor);
	pshader->setFloat4("specularColor", specularColor);
	pshader->setFloat("roughness", roughness);
}


EmiMaterial::EmiMaterial(ShaderClass* pshader): Material(pshader)
{
	emissionColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	intensity = 2.5f;
}

void EmiMaterial::Render()
{
	pshader->use();

	//set color;
	pshader->setFloat4("emissionColor", emissionColor);
	pshader->setFloat("emissionIntensity", intensity);
}