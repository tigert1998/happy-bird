#pragma once

#include <string>
#include <glm/glm.hpp>

#include "shader.h"
#include "color.h"

class Light {
	struct Attenuation{
		float range;
		float constant;
		float linear;
		float quadratic;
		Attenuation(float range = 100): range(range){
			constant = 1;
			linear = 0.5 / constant; // approximation
			quadratic = 90.0 / constant / constant;
		}
		~Attenuation(){ }
	} attenuation_;
public:
	enum{
		kPointLight = 0,
		kParallelLight = 1
	} LightType;
	Light() = default;
	virtual ~Light(){ }
	void Feed(const std::string& identifier, Shader *shader) const;
	inline const Attenuation& attenuation(void) const { return attenuation_; }
};

class PointLight: public Light{
	glm::vec3 position_;
	glm::vec3 color_;
	float intensity_;
 public:
 	PointLight(glm::vec3 position, float intensity = 10, Color color = color::White()): 
 		position_(position), 
 		color_(color),
 		intensity_(intensity){ }
	void Feed(const std::string& identifier, Shader * shader) const;
};

class ParallelLight: public Light{
	glm::vec3 direction_;
	Color color_;
	float intensity_;
 public:
 	ParallelLight(glm::vec3 direction, float intensity = 10, Color color = color::White()): 
 		direction_(direction), 
 		color_(color),
 		intensity_(intensity){ }
	void Feed(const std::string& identifier, Shader * shader) const;
};