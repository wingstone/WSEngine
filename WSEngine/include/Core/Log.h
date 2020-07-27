#pragma once

#include <iostream>
#include <string>

class Log
{
public:
	Log();
	~Log();

	static void log(std::string str);
	static void log(const char* str);
	
private:

};
