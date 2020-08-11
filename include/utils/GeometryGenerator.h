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

    static void GenerateSkyBox(vector<Vertex> &vertices, vector<unsigned int> &indices)
    {
        //bottom
        Vertex v1;
        v1.Position = vec3(-1, -1, 1);
        v1.Texcoord = vec2(0, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, -1, 1);
        v1.Texcoord = vec2(1, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, -1, -1);
        v1.Texcoord = vec2(1, 1);
        vertices.push_back(v1);
        v1.Position = vec3(-1, -1, -1);
        v1.Texcoord = vec2(0, 1);
        vertices.push_back(v1);
        //front
        v1.Position = vec3(-1, -1, -1);
        v1.Texcoord = vec2(0, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, -1, -1);
        v1.Texcoord = vec2(1, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, 1, -1);
        v1.Texcoord = vec2(1, 1);
        vertices.push_back(v1);
        v1.Position = vec3(-1, 1, -1);
        v1.Texcoord = vec2(0, 1);
        vertices.push_back(v1);
        //left
        v1.Position = vec3(-1, -1, 1);
        v1.Texcoord = vec2(0, 0);
        vertices.push_back(v1);
        v1.Position = vec3(-1, -1, -1);
        v1.Texcoord = vec2(1, 0);
        vertices.push_back(v1);
        v1.Position = vec3(-1, 1, -1);
        v1.Texcoord = vec2(1, 1);
        vertices.push_back(v1);
        v1.Position = vec3(-1, 1, 1);
        v1.Texcoord = vec2(0, 1);
        vertices.push_back(v1);
        //right
        v1.Position = vec3(1, -1, -1);
        v1.Texcoord = vec2(0, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, -1, 1);
        v1.Texcoord = vec2(1, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, 1, 1);
        v1.Texcoord = vec2(1, 1);
        vertices.push_back(v1);
        v1.Position = vec3(1, 1, -1);
        v1.Texcoord = vec2(0, 1);
        vertices.push_back(v1);
        //top
        v1.Position = vec3(-1, 1, -1);
        v1.Texcoord = vec2(0, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, 1, -1);
        v1.Texcoord = vec2(1, 0);
        vertices.push_back(v1);
        v1.Position = vec3(1, 1, 1);
        v1.Texcoord = vec2(1, 1);
        vertices.push_back(v1);
        v1.Position = vec3(-1, 1, 1);
        v1.Texcoord = vec2(0, 1);
        vertices.push_back(v1);
        //back
        v1.Position = vec3(1, -1, 1);
        v1.Texcoord = vec2(0, 0);
        vertices.push_back(v1);
        v1.Position = vec3(-1, -1, 1);
        v1.Texcoord = vec2(1, 0);
        vertices.push_back(v1);
        v1.Position = vec3(-1, 1, 1);
        v1.Texcoord = vec2(1, 1);
        vertices.push_back(v1);
        v1.Position = vec3(1, 1, 1);
        v1.Texcoord = vec2(0, 1);
        vertices.push_back(v1);

        //bottom
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
        //front
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(4);
        indices.push_back(6);
        indices.push_back(7);
        //left
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(8);
        indices.push_back(10);
        indices.push_back(11);
        //right
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(12);
        indices.push_back(14);
        indices.push_back(15);
        //top
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(16);
        indices.push_back(18);
        indices.push_back(19);
        //back
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(20);
        indices.push_back(22);
        indices.push_back(23);
    }
};
