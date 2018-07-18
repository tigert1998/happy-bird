#include "debug_utility/log.h"

void Log(std::string function, int line, std::string file) {
	printf("[%s] line %d, file %s\n", function.c_str(), line, file.c_str());
}