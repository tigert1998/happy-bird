#include "shader_utility/light.h"
#include "shader_utility/attenuation.h"

class PointLight: public Light {
private:
	glm::vec3 position_;
	Attenuation attenuation_;
	
public:
	PointLight(glm::vec3 position, glm::vec3 color, float intensity, const Attenuation& attenuation);
	LightType type() const; 
	glm::vec3 position() const;
	Attenuation attenuation() const;
};