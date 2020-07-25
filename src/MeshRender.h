#ifndef MESHRENDER_H
#define MESHRENDER_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "Transform.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

class MeshRender
{

public:
	MeshRender(string path)
	{
		LoadModel(path);
	}

	void DrawModel(Transform* transform, Camera* camera, Material* overrideMaterial, Light *light, AmbientLight * amlight)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			meshes[i].DrawMesh(transform, overrideMaterial, camera, light, amlight);
		}
	}


	~MeshRender()
	{
	}

private:
	string directory;
	vector<Mesh> meshes;

private:
	void LoadModel(string path)
	{
		Assimp::Importer impoter;
		const aiScene *scene = impoter.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << impoter.GetErrorString() << endl;
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode *node, const aiScene *scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;

		//处理顶点
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			if (mesh->mColors[0])
			{
				vertex.Color = vec4(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b, mesh->mColors[0][i].a);
			}
			else
			{
				vertex.Color = vec4(1.0f, 1.0f, 1.0f, 0.0f);
			}
			vertex.Normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			if (mesh->mTextureCoords[0])
			{
				vertex.Texcoord = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertex.Texcoord = vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		//处理顶点索引
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		return Mesh(vertices, indices);
	}

};


#endif