#include "cg_exception.h"

FileNotExistsError::FileNotExistsError(const std::string &path) {
	error_message = "[file error] File not exists at " + path;
}

const char *FileNotExistsError::what() const noexcept {
    return error_message.c_str();
}

ShaderCompileError::ShaderCompileError(const std::string &title, const std::string &log) {
	error_message = "[shader compile error on " + title + "] " + log;
}

const char *ShaderCompileError::what() const noexcept {
	return error_message.c_str();
}

ShaderLinkError::ShaderLinkError(const std::string &log) {
	error_message = "[shader link error] " + log;
}

const char *ShaderLinkError::what() const noexcept {
	return error_message.c_str();
}

LoadPictureError::LoadPictureError(const std::string &path) {
	error_message = "[picture format error] Fail to load picture at " + path;
}

const char *LoadPictureError::what() const noexcept {
	return error_message.c_str();
}

AssimpError::AssimpError(const std::string &error_string) {
	error_message = "[assimp error] " + error_string;
}

const char *AssimpError::what() const noexcept {
	return error_message.c_str();
}

ShaderSettingError::ShaderSettingError(const std::string &name) {
	error_message = "[shader setting error] Fail to set uniform variable " + name;
}

const char *ShaderSettingError::what() const noexcept {
	return error_message.c_str();
}