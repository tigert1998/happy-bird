#include "shader_utility/point_light.h"
#include "shader_utility/parallel_light.h"

class SpotLight: public PointLight, public ParallelLight {
private:
	float angle_;
public:
	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float intensity, const Attenuation& attenuation, float angle);
	float angle() const;
	LightType type() const;
};