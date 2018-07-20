#include "file_manager.h"

const FileManager FileManager::shared = FileManager();

FileManager::FileManager(): work_path(""){
    if(!FileExistsAt(work_path + "shader/common.frag")){
        work_path = "D:/code/happy-bird/";
    }
}

bool FileManager::FileExistsAt(const std::string &path) const {
	 std::ifstream is( path);
    return is.is_open();
}

void FileManager::CreateFileAt(const std::string &path) const {
    std::ofstream os(work_path + path);
}

uint64_t FileManager::FileSizeAt(const std::string &path) const {
    using namespace std;
    fstream fs;
    fs.open(work_path + path, ios::in | ios::binary);
    fs.seekg(0, ios::end);
    return (uint64_t) fs.tellg();
}

void FileManager::RemoveFileAt(const std::string &path) const {
    auto full_path = work_path + path;
    remove(full_path.c_str());
}

const std::string FileManager::FileContentAt(const std::string &path) const {
    using namespace std;
    if (!FileExistsAt(work_path + path)) throw FileNotExistsError(path);
    ifstream in(work_path + path);
    istreambuf_iterator<char> begin(in);
    istreambuf_iterator<char> end;
    string res(begin, end);
    return res;
}

