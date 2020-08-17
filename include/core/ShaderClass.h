#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace glm;

class ShaderClass
{
public:
	ShaderClass();

	ShaderClass(const GLchar* vertContent, const GLchar* fragContent, bool isFilePath);

	~ShaderClass();

	void use();

	void setBool(const string &name, bool value) const;

	void setInt(const string &name, int value) const;

	void setFloat(const string &name, float value) const;
	
	void setFloat2(const string &name, float value1, float value2) const;

	void setFloat2(const string &name, vec2 parm2) const;

	void setFloat4(const string &name, float value1, float value2, float value3, float value4) const;

	void setFloat4(const string &name, vec4 parm4) const;

	void setTexture(const string &name, unsigned int ID, unsigned int slot, bool isCube = false) const;

	unsigned int ID;

private:

};

#endif // !SHADER_H
