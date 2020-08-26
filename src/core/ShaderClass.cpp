#include <core/ShaderClass.h>

ShaderClass::ShaderClass()
{
	ID = 0;
}

ShaderClass::ShaderClass(string name, const GLchar *vertContent, const GLchar *fragContent, bool isFilePath)
{
	this->name = name;
	//读取shader文件
	string vertexCode;
	string fragmentCode;
	const char *vShaderCode;
	const char *fShaderCode;
	if (isFilePath)
	{

		ifstream vShaderFile;
		ifstream fShaderFile;
		vShaderFile.exceptions(ifstream::failbit || ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit || ifstream::badbit);
		try
		{
			vShaderFile.open(vertContent);
			fShaderFile.open(fragContent);
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

		vShaderCode = vertexCode.c_str();
		fShaderCode = fragmentCode.c_str();
	}
	else
	{
		vShaderCode = vertContent;
		fShaderCode = fragContent;
	}

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
		cout << "Shader: " << name << "\n"
			 << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			 << infolog << endl;
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
		cout << "Shader: " << name << "\n"
			 << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			 << infolog << endl;
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
		cout << "Shader: " << name << "\n"
			 << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
			 << infolog << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderClass::~ShaderClass()
{
}

void ShaderClass::use()
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

void ShaderClass::setBool(const string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ShaderClass::setInt(const string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderClass::setFloat(const string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderClass::setFloat2(const string &name, float value1, float value2) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}

void ShaderClass::setFloat2(const string &name, vec2 parm2) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), parm2.x, parm2.y);
}

void ShaderClass::setFloat4(const string &name, float value1, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

void ShaderClass::setFloat4(const string &name, vec4 parm4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), parm4.x, parm4.y, parm4.z, parm4.w);
}

void ShaderClass::setTexture(const string &name, unsigned int ID, unsigned int slot, bool isCube) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	setInt(name, slot);
	if (isCube)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}
}