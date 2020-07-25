# include<Log>

void Log::log(std::string str)
{
	std::cout << str << std::endl;
}

void Log::log(const char* str)
{
	std::cout << str << std::endl;
}

Log::Log()
{
}

Log::~Log()
{
}