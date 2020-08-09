#ifndef GLOBALLOG_H
#define GLOBALLOG_H

#include <glm/glm.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace glm;

class GlobalLog
{
public:
	static void Log(const char *str)
	{
		cout << str << endl;
	}

	static void Log(string str)
	{
		cout << str << endl;
	}

	static void Log(vec3 v3)
	{
		cout << v3.x << "," << v3.y << "," << v3.z << endl;
	}

	static void Log(vec4 v4)
	{
		cout << v4.x << "," << v4.y << "," << v4.z << "," << v4.w << endl;
	}

	static void Log(const char *str, float v)
	{
		cout << str << ": " << v << endl;
	}

	static void Log(const char *str, vec4 v4)
	{
		cout << str << ": " << v4.x << "," << v4.y << "," << v4.z << "," << v4.w << endl;
	}

private:
	GlobalLog();
};

#endif