#include <string>

#define LOG() Log(__FUNCTION__, __LINE__, __FILE__)

void Log(std::string function, int line, std::string file);