#include <memory>

#include "shader_utility/light.h"
#include "shader_utility/attenuation.h"

class Object;

class PointLight: virtual public Light {
 protected:
	glm::vec3 position_;
	Attenuation attenuation_;
	std::weak_ptr<Object> anchor_;
 public:
	PointLight(glm::vec3 position, glm::vec3 color, float intensity, const Attenuation& attenuation);
	void Attach(std::weak_ptr<Object>);
	LightType type() const; 
	glm::vec3 position() const;
	Attenuation attenuation() const;
};