#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

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
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);

	void DrawMesh();

	~Mesh();

private:
	unsigned int VAO, VBO, IBO;

private:
	void SetupMesh();

};

#endif