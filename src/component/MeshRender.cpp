#include <component/MeshRender.h>

ComponentType MeshRender::type() const
{
	return TYPE;
}

MeshRender::MeshRender()
{
	transform = nullptr;
	mesh = nullptr;
	material = nullptr;
}

void MeshRender::SetMesh(Mesh *mesh)
{
	this->mesh = mesh;
}

void MeshRender::SetMaterial(Material *material)
{
	this->material = material;
}

void MeshRender::DrawModel(Transform *transform, Camera *camera, Light *light, LightSetting *lightSetting, RenderTexture *shadowMap)
{
	material->ImportRenderSetting(transform, camera, light, lightSetting, shadowMap);
	material->Render();

	mesh->DrawMesh();
}