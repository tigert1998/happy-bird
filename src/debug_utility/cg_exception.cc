#include <iostream>

#include "debug_utility/cg_exception.h"

FileNotExistsError::FileNotExistsError(const std::string &path) {
	error_message = "[file error] File not exists at " + path;
	std::cout << error_message << std::endl;
}

const char *FileNotExistsError::what() const noexcept {
    return error_message.c_str();
}

ShaderCompileError::ShaderCompileError(const std::string &title, const std::string &log) {
	error_message = "[shader compile error on " + title + "] " + log;
	std::cout << error_message << std::endl;
}

const char *ShaderCompileError::what() const noexcept {
	return error_message.c_str();
}

ShaderLinkError::ShaderLinkError(const std::string &log) {
	error_message = "[shader link error] " + log;
	std::cout << error_message << std::endl;
}

const char *ShaderLinkError::what() const noexcept {
	return error_message.c_str();
}

LoadPictureError::LoadPictureError(const std::string &path) {
	error_message = "[picture format error] Fail to load picture at " + path;
	std::cout << error_message << std::endl;
}

const char *LoadPictureError::what() const noexcept {
	return error_message.c_str();
}

AssimpError::AssimpError(const std::string &error_string) {
	error_message = "[assimp error] " + error_string;
	std::cout << error_message << std::endl;
}

const char *AssimpError::what() const noexcept {
	return error_message.c_str();
}

ShaderSettingError::ShaderSettingError(const std::string &name) {
	error_message = "[shader setting error] Fail to set uniform variable " + name;
	std::cout << error_message << std::endl;
}

const char *ShaderSettingError::what() const noexcept {
	return error_message.c_str();
}