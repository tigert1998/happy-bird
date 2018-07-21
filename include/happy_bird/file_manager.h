#pragma once

#include "debug_utility/cg_exception.h"

#include <fstream>
#include <string>

class FileManager {
 public:
	std::string work_path;
	const static FileManager shared;
	FileManager();
	bool FileExistsAt(const std::string &) const;
	void CreateFileAt(const std::string &) const;
	uint64_t FileSizeAt(const std::string &) const;
	void RemoveFileAt(const std::string &) const;
	const std::string FileContentAt(const std::string &) const;
};
