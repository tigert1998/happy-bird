#pragma once

#include "cg_exception.h"

#include <fstream>
#include <string>

class FileManager {
public:
    const static FileManager shared;
    bool FileExistsAt(const std::string &) const;
    void CreateFileAt(const std::string &) const;
    uint64_t FileSizeAt(const std::string &) const;
    void RemoveFileAt(const std::string &) const;
    const std::string FileContentAt(const std::string &) const;
    
};
