#pragma once

#include <string>
#include <glm/glm.hpp>

enum class LightType {
	kPoint, kParallel
};

class Light {
private:
	glm::vec3 color_;
	float intensity_;

public:
	virtual LightType type() const = 0;
	Light(glm::vec3 color, float intensity);
	glm::vec3 color() const;
	float intensity() const;
};