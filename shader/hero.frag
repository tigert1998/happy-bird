#version 330 core

const float zero = 0.00000001;

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
	ParallelLight parallel[4];
	PointLight point[4];
	SpotLight spot[4];
};

struct TextureMaterial {
	float shininess;
	sampler2D diffuseTexture;
	sampler2D specularTexture;
};

struct Eye {
	vec3 position;
};

uniform Eye uEye;
uniform LightCollection uLightCollection;
uniform TextureMaterial uMaterial;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 fragColor;

vec3 CalculatePointLight(Eye eye, PointLight light, TextureMaterial material) {
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
	
	vec3 diffuseColor = diffuseFactor * texture(material.diffuseTexture, vTexCoord).rgb * light.color;
	vec3 specularColor = specularFactor * texture(material.specularTexture, vTexCoord).rgb * light.color ;
	return attenuation * (diffuseColor + specularColor);
}

vec3 CalculateParallelLight(Eye eye, ParallelLight light, TextureMaterial material) {
	vec3 lightDirection = normalize(-light.direction);
	float diffuseFactor = max(dot(vNormal, lightDirection), 0.0) * light.intensity;

	vec3 reflectDirection = normalize(reflect(-lightDirection, vNormal));
	vec3 viewDirection = normalize(eye.position - vPosition);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess) * light.intensity;
	
	vec3 diffuseColor = diffuseFactor * texture(material.diffuseTexture, vTexCoord).rgb * light.color;
	vec3 specularColor = specularFactor * texture(material.specularTexture, vTexCoord).rgb * light.color;
	return diffuseColor + specularColor;
}

vec3 CalculateSpotLight(Eye eye, SpotLight light, TextureMaterial material) {
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
	
	vec3 diffuseColor = diffuseFactor * texture(material.diffuseTexture, vTexCoord).rgb * light.color;
	vec3 specularColor = specularFactor * texture(material.specularTexture, vTexCoord).rgb * light.color;
	return attenuation * spotFactor * (diffuseColor + specularColor);
}

vec3 CalculateFragmentColor(Eye eye, LightCollection lightCollection, TextureMaterial material) {
	vec3 color = lightCollection.ambient;
	for (int i = 0; i < lightCollection.total.parallel; i++) {
		if(i == 0){
			color += CalculateParallelLight(eye, lightCollection.parallel[0], material);
		}
		if(i == 1){
			color += CalculateParallelLight(eye, lightCollection.parallel[1], material);
		}
		if(i == 2){
			color += CalculateParallelLight(eye, lightCollection.parallel[2], material);
		}
		if(i == 3){
			color += CalculateParallelLight(eye, lightCollection.parallel[3], material);
		}
	}
	for (int i = 0; i < lightCollection.total.point; i++) {
		if(i == 0){
			color += CalculatePointLight(eye, lightCollection.point[0], material);
		}
		if(i == 1){
			color += CalculatePointLight(eye, lightCollection.point[1], material);
		}
		if(i == 2){
			color += CalculatePointLight(eye, lightCollection.point[2], material);
		}
		if(i == 3){
			color += CalculatePointLight(eye, lightCollection.point[3], material);
		}
	}
	for (int i = 0; i < lightCollection.total.spot; i++) {
		if(i == 0){
			color += CalculateSpotLight(eye, lightCollection.spot[0], material);
		}
		if(i == 1){
			color += CalculateSpotLight(eye, lightCollection.spot[1], material);
		}
		if(i == 2){
			color += CalculateSpotLight(eye, lightCollection.spot[2], material);
		}
		if(i == 3){
			color += CalculateSpotLight(eye, lightCollection.spot[3], material);
		}
	}
	return color * 5;
}

void main() {
	fragColor = vec4(CalculateFragmentColor(uEye, uLightCollection, uMaterial), texture(uMaterial.diffuseTexture, vTexCoord).a);
}