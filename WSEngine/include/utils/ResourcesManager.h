#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>

#include "../core/Texture.h"
#include "../core/Mesh.h"
#include "../core/Material.h"
#include "../core/RenderTexture.h"

#include "GlobalLog.h"

class WSEngine;

class ResourcesManager
{
private:
	std::map<string, Texture *> textures;
	std::map<string, Mesh *> meshs;
	std::map<string, ShaderClass *> shaders;
	std::map<string, Material *> materials;
	std::map<string, RenderTexture *> renderTextures;

	ResourcesManager()
	{
	}

public:
	static ResourcesManager &Instance()
	{
		static ResourcesManager instance;
		return instance;
	}

	void Init()
	{
	}

	void Quit()
	{
		for (auto iter = textures.begin(); iter != textures.end(); iter++)
		{
			textures.erase(iter);
			delete iter->second;
		}
		for (auto iter = meshs.begin(); iter != meshs.end(); iter++)
		{
			meshs.erase(iter);
			delete iter->second;
		}
		for (auto iter = shaders.begin(); iter != shaders.end(); iter++)
		{
			shaders.erase(iter);
			delete iter->second;
		}
		for (auto iter = materials.begin(); iter != materials.end(); iter++)
		{
			materials.erase(iter);
			delete iter->second;
		}
		for (auto iter = renderTextures.begin(); iter != renderTextures.end(); iter++)
		{
			renderTextures.erase(iter);
			delete iter->second;
		}
	}

	Texture *LoadTexture(const char *path, const char *name)
	{
		//check name
		if (textures.find(name) != textures.end())
		{
			GlobalLog::Log("can't find texture: " + std::string(name));
			return nullptr;
		}

		Texture *tex = new Texture();

		glGenTextures(1, &(tex->ID));
		glBindTexture(GL_TEXTURE_2D, tex->ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		int width1, height1, nChannels1;
		stbi_set_flip_vertically_on_load(true); //invert y
		unsigned char *data1 = stbi_load(path, &width1, &height1, &nChannels1, 0);

		if (data1 != NULL)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "Failed to load texture" << endl;
		}

		stbi_image_free(data1);

		//manage
		textures[name] = tex;
		return tex;
	}

	Texture *LoadCubeTexture(vector<string> paths, const char *name)
	{
		//check name
		if (textures.find(name) != textures.end())
		{
			GlobalLog::Log("can't find texture: " + std::string(name));
			return nullptr;
		}

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < paths.size(); i++)
		{
			unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
							 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//manage
		Texture *tex = new Texture();
		tex->ID = textureID;
		textures[name] = tex;
		return tex;
	}

	std::vector<Mesh *> LoadMesh(const char *path, vector<string> *outName)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return std::vector<Mesh *>();
		}

		std::vector<Mesh *> loadmeshs;

		processNode(scene->mRootNode, scene, &loadmeshs, outName);

		return loadmeshs;
	}

	Mesh *CreateMesh(vector<Vertex> vertices, vector<unsigned int> indices, const char *name)
	{
		//check name
		if (meshs.find("name") != meshs.end())
		{
			GlobalLog::Log("can't load mesh" + string(name));
			return nullptr;
		}

		Mesh *pmesh = new Mesh(vertices, indices);
		meshs[name] = pmesh;
		return pmesh;
	}

	ShaderClass *CreateShader(const char *vertContent, const char *fragContent, const char *name)
	{
		//check name
		if (shaders.find("name") != shaders.end())
		{
			GlobalLog::Log("can't load shader" + string(name));
			return nullptr;
		}

		ShaderClass *shader = new ShaderClass(vertContent, fragContent, false);
		shaders[name] = shader;
		return shader;
	}

	ShaderClass *LoadShader(const char *vertFile, const char *fragFile, const char *name)
	{
		//check name
		if (shaders.find("name") != shaders.end())
		{
			GlobalLog::Log("can't load shader" + string(name));
			return nullptr;
		}

		ShaderClass *shader = new ShaderClass(vertFile, fragFile, true);
		shaders[name] = shader;
		return shader;
	}

	template <class Type>
	Type *CreateMaterial(ShaderClass *shader, const char *name)
	{
		//check name
		if (materials.find("name") != materials.end())
		{
			GlobalLog::Log("can't create material" + string(name));
			return nullptr;
		}

		Type *material = new Type(shader);
		materials[name] = dynamic_cast<Material *>(material);
		return material;
	}

	RenderTexture *CreateRenderTexture(unsigned int width, unsigned int height, const char *name,
									   bool haveRenderBuffer = true, bool isHDR = false,
									   bool isMSAA = false, unsigned int msaaSample = 4)
	{
		//check name
		if (renderTextures.find("name") != renderTextures.end())
		{
			GlobalLog::Log("can't create material" + string(name));
			return nullptr;
		}
		// framebuffer configuration
		// -------------------------
		unsigned int framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// create a color attachment texture
		unsigned int textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		if (isHDR && isMSAA)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSample, GL_RGB16F, width, height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer, 0);
		}
		else if (isHDR && !isMSAA)
		{
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//must have
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (!isHDR && isMSAA)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorbuffer, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //must have
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		unsigned int rbo = 0;
		if (haveRenderBuffer)
		{
			if (!isMSAA)
			{
				glGenRenderbuffers(1, &rbo);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			}
			else
			{
				glGenRenderbuffers(1, &rbo);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSample, GL_DEPTH24_STENCIL8, width, height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
			}
		}

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		RenderTexture *rt = new RenderTexture();
		renderTextures[name] = rt;
		rt->framebuffer = framebuffer;
		rt->textureColorbuffer = textureColorbuffer;
		rt->rbo = rbo;
		return rt;
	}

private:
	void processNode(aiNode *node, const aiScene *scene, std::vector<Mesh *> *loadmeshs, vector<string> *outName)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene, loadmeshs, outName);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, loadmeshs, outName);
		}
	}

	void processMesh(aiMesh *mesh, const aiScene *scene, std::vector<Mesh *> *loadmeshs, vector<string> *outName)
	{
		string name = mesh->mName.C_Str();

		//check name
		if (meshs.find("name") != meshs.end())
		{
			GlobalLog::Log("can't load mesh" + string(name));
			return;
		}

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

		Mesh *pmesh = new Mesh(vertices, indices);
		meshs[name] = pmesh;
		loadmeshs->push_back(pmesh);
		outName->push_back(name);
	}
};

#endif