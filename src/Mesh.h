#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

using namespace std;
using namespace glm;

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec3 Normal;
	glm::vec2 Texcoord;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;

public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;
		SetupMesh();
	}

	void DrawMesh(Transform *transform, Material *material, Camera *camera, Light *light, AmbientLight *amlight)
	{
		//设置材质shader
		material->Use();

		//传递mvp参数
		unsigned int modelLoc = glGetUniformLocation(material->pshader->ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(transform->GetModelMatrix()));
		unsigned int viewLoc = glGetUniformLocation(material->pshader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));
		unsigned int projectionLoc = glGetUniformLocation(material->pshader->ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(camera->GetProjectionMatrix()));

		//传递光照相关参数
		material->pshader->setFloat4("cameraPos", vec4(camera->Position, 0.0f));

		material->pshader->setFloat4("lightPos", light->lightPos);
		material->pshader->setFloat4("lightColor", light->lightColor);

		material->pshader->setFloat4("ambientColor", amlight->ambientColor);
		material->pshader->setFloat("ambientIntensity", amlight->ambientIntensity);


		//绘制网格
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	~Mesh()
	{
	}

private:
	unsigned int VAO, VBO, IBO;

private:
	void SetupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Texcoord));

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

};

#endif