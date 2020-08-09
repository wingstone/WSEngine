#pragma once

#include <vector>

#include "../core/Mesh.h"

using namespace std;
using namespace glm;

class GeometryGenerator
{
public:
    static void GenerateQuad(vector<Vertex> &vertices, vector<unsigned int> &indices)
    {
        Vertex v1;
        v1.Position = vec3(-1, -1, 0);
        v1.Texcoord = vec2(0, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, -1, 0);
        v1.Texcoord = vec2(1, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, 1, 0);
        v1.Texcoord = vec2(1, 1);
        vertices.push_back(v1);
        v1.Position = vec3(-1, 1, 0);
        v1.Texcoord = vec2(0, 1);
        vertices.push_back(v1);

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
    }
};
