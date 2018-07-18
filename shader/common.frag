#version 330 core

struct Attenuation {
	float range;
	float constant;
	float linear;
	float quadratic;
};

struct ParallelLight {
	vec3 direction;
	vec3 color;
	float intensity;
};

struct SpotLight {
	float angle;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	Attenuation attenuation;
};

struct PointLight {
	vec3 position;
	Attenuation attenuation;
	vec3 color;
	float intensity;
};

struct LightCollectionTotal {
	int parallel;
	int point;
	int spot;
};

struct LightCollection {
	vec3 ambient;
	LightCollectionTotal total;
	ParallelLight parallel[1];
	PointLight point[1];
	SpotLight spot[1];
};

struct Material {
	float shininess;
	vec3 diffuse;
	vec3 specular;
};

struct Eye {
	vec3 position;
};

uniform Eye uEye;
uniform LightCollection uLightCollection;
uniform Material uMaterial;

in vec3 vPosition;
in vec3 vNormal;

out vec4 fragColor;

vec3 CalculatePointlLight(Eye eye, PointLight light, Material material) {
	vec3 lightDirection = normalize(light.position - vPosition);
	float diffuseFactor = max(dot(vNormal, lightDirection), 0.0) * light.intensity;

	vec3 reflectDirection = normalize(reflect(-lightDirection, vNormal));
	vec3 viewDirection = normalize(eye.position - vPosition);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) * light.intensity;

    float distance = length(light.position - vPosition);
	float attenuation = 0.0f;
	if (distance < light.attenuation.range) {
		attenuation = 1.0f / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * pow(distance, 2));  
	}
	
	vec3 diffuseColor = diffuseFactor * material.diffuse * light.color;
	vec3 specularColor = specularFactor * material.specular * light.color ;
	return attenuation * (diffuseColor + specularColor);
}

vec3 CalculateParallelLight(Eye eye, ParallelLight light, Material material) {
	vec3 lightDirection = normalize(-light.direction);
	float diffuseFactor = max(dot(vNormal, lightDirection), 0.0) * light.intensity;

	vec3 reflectDirection = normalize(reflect(-lightDirection, vNormal));
	vec3 viewDirection = normalize(eye.position - vPosition);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) * light.intensity;
	
	vec3 diffuseColor = diffuseFactor * material.diffuse * light.color;
	vec3 specularColor = specularFactor * material.specular * light.color;
	return diffuseColor + specularColor;
}

vec3 CalculateSpotLight(Eye eye, SpotLight light, Material material) {
	vec3 lightDirection = normalize(light.position - vPosition);
	float diffuseFactor = max(dot(vNormal, lightDirection), 0.0) * light.intensity;

	float spotFactor = max(1.0 - (1.0 - max(dot(-lightDirection, normalize(light.direction)), 0)) / (1.0 - cos(light.angle)), 0);

	vec3 reflectDirection = normalize(reflect(-lightDirection, vNormal));
	vec3 viewDirection = normalize(eye.position - vPosition);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) * light.intensity;

    float distance = length(light.position - vPosition);
	float attenuation = 0.0f;
	if (distance < light.attenuation.range) {
		attenuation = 1.0f / (light.attenuation.constant + light.attenuation.linear * distance + light.attenuation.quadratic * pow(distance, 2));  
	}
	
	vec3 diffuseColor = diffuseFactor * material.diffuse * light.color;
	vec3 specularColor = specularFactor * material.specular * light.color ;
	return attenuation * spotFactor * (diffuseColor + specularColor);
}

vec3 CalculateFragmentColor(Eye eye, LightCollection lightCollection, Material material) {
	vec3 color = lightCollection.ambient;
	for (int i = 0; i < lightCollection.total.parallel; i++) {
		color += CalculateParallelLight(eye, lightCollection.parallel[i], material);
	}
	for (int i = 0; i < lightCollection.total.point; i++) {
		color += CalculatePointlLight(eye, lightCollection.point[i], material);
	}
	for (int i = 0; i < lightCollection.total.spot; i++) {
		color += CalculateSpotLight(eye, lightCollection.spot[i], material);
	}
	return color;
}

void main() {
	fragColor = vec4(CalculateFragmentColor(uEye, uLightCollection, uMaterial), 1);
}