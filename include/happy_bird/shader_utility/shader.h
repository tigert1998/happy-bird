#pragma once

#include <string>

#define DEBUG

#include "file_manager.h"
#include "opengl_common.h"
#include "bullet_common.h"

#ifndef _MSC_VER
#define cloth_vert "shader/cloth.vert"
#define cloth_frag "shader/cloth.frag"
#define common_vert "shader/common.vert"
#define common_frag "shader/common.frag"
#define hero_vert "shader/hero.vert"
#define hero_frag "shader/hero.frag"
#define particle_vert "shader/particle.vert"
#define particle_frag "shader/particle.frag"
#else 
#define cloth_vert "D:\\code\\happy-bird\\shader\\cloth.vert"
#define cloth_frag "D:\\code\\happy-bird\\shader\\cloth.frag"
#define common_vert "D:\\code\\happy-bird\\shader\\common.vert"
#define common_frag "D:\\code\\happy-bird\\shader\\common.frag"
#define hero_vert "D:\\code\\happy-bird\\shader\\hero.vert"
#define hero_frag "D:\\code\\happy-bird\\shader\\hero.frag"
#define particle_vert "D:\\code\\happy-bird\\shader\\particle.vert"
#define particle_frag "D:\\code\\happy-bird\\shader\\particle.frag"
#endif

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

