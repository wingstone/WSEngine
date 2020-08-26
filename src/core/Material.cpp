#include <core/Material.h>

void Material::ImportRenderSetting(Transform *transform, Camera *camera, Light *light, LightSetting *lightSetting, RenderTexture *shadowMap)
{
	pshader->use();

	//matrix
	unsigned int modelLoc = glGetUniformLocation(pshader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(transform->GetModelMatrix()));
	unsigned int viewLoc = glGetUniformLocation(pshader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));
	unsigned int projectionLoc = glGetUniformLocation(pshader->ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(camera->GetProjectionMatrix()));

	//shadow
	if (shadowMap != 0)
	{
		mat4 proj = light->GetShadowMatrix();
		unsigned int shadowProjLoc = glGetUniformLocation(pshader->ID, "shadowProject");
		glUniformMatrix4fv(shadowProjLoc, 1, GL_FALSE, value_ptr(proj));
		pshader->setTexture("ShadowMap", shadowMap->textureColorbuffer, 0);
		int width = shadowMap->width;
		int height = shadowMap->height;
		pshader->setFloat4("ShadowMap_TextureSize", vec4(width, height, 1.0f/width, 1.0f/height));
	}
	
	pshader->setFloat4("cameraPos", vec4(camera->entity->GetComponent<Transform>()->position, 0.0f));

	vec3 lightdir = light->transform->Front();
	//light
	if (light->lighttype == Light::LightType::DIRECTIONAL)
	{
		pshader->setFloat4("lightPos", vec4(-lightdir, 0.0f));
	}
	else
	{
		pshader->setFloat4("lightPos", vec4(lightdir, 1.0f));
	}
	pshader->setFloat4("lightColor", light->lightColor);
	pshader->setFloat("lightIntensity", light->lightIntensity);

	pshader->setFloat4("ambientColor", lightSetting->ambientColor);
	pshader->setFloat("ambientIntensity", lightSetting->ambientIntensity);
}

PBRMaterial::PBRMaterial(ShaderClass *pshader) : Material(pshader)
{
	this->textures = vector<Texture *>();
	this->strings = vector<string>();
	strings.push_back("albedoTex");
	strings.push_back("metalicTex");
	strings.push_back("normalTex");
	diffuseColor = vec4(0.9f, 0.6f, 0.8f, 1.0f);
	specularColor = vec4(0.4f, 0.6f, 0.3f, 1.0f);
	roughness = 0.7f;
}

void PBRMaterial::SetShadowMap(Texture *shadowmap)
{
	this->shadowmap = shadowmap;
}

void PBRMaterial::Render()
{
	pshader->use();
	//set texture
	unsigned int i = 0;
	for (; i < glm::min(textures.size(), strings.size()); i++)
	{
		pshader->setTexture(strings[i], textures[i]->ID, i + 1);
	}

	//set color;
	pshader->setFloat4("diffuseColor", diffuseColor);
	pshader->setFloat4("specularColor", specularColor);
	pshader->setFloat("roughness", roughness);
}

EmiMaterial::EmiMaterial(ShaderClass *pshader) : Material(pshader)
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