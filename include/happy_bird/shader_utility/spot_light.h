#include <memory>

#include "shader_utility/point_light.h"
#include "shader_utility/parallel_light.h"

class Object;

class SpotLight: public PointLight, public ParallelLight {
 protected:
	float angle_;
 public:
	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, const Attenuation& attenuation, float angle);
	void Attach(std::weak_ptr<Object>);
	float angle() const;
	glm::vec3 direction(void) const;
	LightType type() const;
};