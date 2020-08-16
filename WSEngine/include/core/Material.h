#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "../core/Texture.h"
#include "../core/ShaderClass.h"

#include "../component/Transform.h"
#include "../component/Camera.h"
#include "../component/Light.h"
#include "LightSetting.h"

using namespace std;
using namespace glm;

struct LightSetting;

class Material
{
protected:
	ShaderClass* pshader;

public:
	Material(ShaderClass* pshader)
	{
		this->pshader = pshader;
	}
	void ImportRenderSetting(Transform* transform, Camera* came, Light* light, LightSetting* lightSetting);
	virtual void Render() = 0;

};

class PBRMaterial : public Material
{
public:
	vector<Texture*> textures;
	vector<string> strings;
	vec4 diffuseColor;
	vec4 specularColor;
	float roughness;

	PBRMaterial(ShaderClass* pshader);

	void Render();
};

#endif