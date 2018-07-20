#pragma once

#include "shader_utility/material.h"
#include <string>

class TextureMaterial: public Material {
private:
	uint32_t diffuse_texture_;
	uint32_t specular_texture_;

	static uint32_t LoadTexture(const std::string& path);
public:
	TextureMaterial() = delete;
	TextureMaterial(const std::string& diffuse_texture_path, const std::string& specular_texture_path, float shininess);
	uint32_t diffuse_texture() const;
	uint32_t specular_texture() const;
	MaterialType type() const;
};