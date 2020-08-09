#ifndef MESHRENDER_H
#define MESHRENDER_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "../core/Mesh.h"
#include "../core/Component.h"
#include "../core/Material.h"

using namespace std;
using namespace glm;

class Transform;

class MeshRender: public Component
{

public:
	static const ComponentType TYPE = COMPONENT_RENDERER;
	Transform* transform;
	Mesh* mesh;
	Material* material;
	
	ComponentType type() const;

	MeshRender();

	void SetMesh(Mesh* mesh);

	void SetMaterial(Material* material);

	void DrawModel(Transform* transform, Camera* camera, Light *light, LightSetting * lightSetting);

};


#endif