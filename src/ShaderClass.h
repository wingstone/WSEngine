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
	ShaderClass()
	{
		ID = 0;
	}

	ShaderClass(const GLchar* vertPath, const GLchar* fragPath)
	{
		//读取shader文件
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		vShaderFile.exceptions(ifstream::failbit || ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit || ifstream::badbit);
		try
		{
			vShaderFile.open(vertPath);
			fShaderFile.open(fragPath);
			stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (const std::ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		int success = 0;
		char infolog[512];
		//顶点着色器
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << endl;
		}

		//片段着色器
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << endl;
		}

		//着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infolog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infolog << endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	~ShaderClass()
	{
	}


	void use()
	{
		if (ID != 0)
		{
			glUseProgram(ID);
		}
		else
		{
			throw exception("No shader to use!");
		}
	}
	void setBool(const string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat4(const string &name, float value1, float value2, float value3, float value4) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
	}

	void setFloat4(const string &name, vec4 parm4) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), parm4.x, parm4.y, parm4.z, parm4.w);
	}

	unsigned int ID;

private:

};

#endif // !SHADER_H
