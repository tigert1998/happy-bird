#pragma once

#include <string>
#include <glm/glm.hpp>

class Object;

enum class LightType {
	kPoint, kParallel, kSpot
};

class Light {
private:
	glm::vec3 color_;
	float intensity_;

public:
	virtual LightType type() const = 0;
	Light(glm::vec3 color, float intensity);
	virtual void Attach(Object*) = 0;
	glm::vec3 color() const;
	float intensity() const;
};