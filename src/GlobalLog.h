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
	GlobalLog();
	~GlobalLog();

	static void Log(const char* str)
	{
		cout << str << endl;
	}

	static void Log(const char* str, float v)
	{
		cout << str << ": " << v << endl;
	}

	static void Log(const char* str,vec4 v4)
	{
		cout << str << ": " << v4.x << "," << v4.y << "," << v4.z << "," << v4.w << endl;
	}
	
private:

};

GlobalLog::GlobalLog()
{
}

GlobalLog::~GlobalLog()
{
}

#endif