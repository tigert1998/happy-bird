#pragma once

#define DEBUG

#include "file_manager.h"
#include "bullet_common.h"

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
	Shader() = delete;
	Shader(const std::string &vs_path, const std::string &fs_path);
	void Use() const;
	template <typename T> void SetUniform(const std::string &identifier, const T&) const;

private:
	const FileManager file_manager = FileManager();

	static uint32_t Compile(GLenum type, const std::string &source, const std::string &path);
	static uint32_t Link(uint32_t vs_id, uint32_t fs_id);

	uint32_t id;
};

