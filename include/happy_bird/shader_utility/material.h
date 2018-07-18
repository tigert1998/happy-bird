#pragma once

#include <glm/glm.hpp>

enum class MaterialType {
	kPureColor, kTexture
};

class Material {
private:
	float shininess_;
public:
	Material() = delete;
	Material(float shininess);
	float shininess() const;
	virtual MaterialType type() const = 0;
};