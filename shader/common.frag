#version 330 core

struct Light {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Eye {
	vec3 position;
};

uniform Eye uEye;
uniform Light uLight;
uniform vec3 uColor;

in vec3 vPosition;
in vec3 vNormal;

out vec4 fragColor;

void main() {
	vec3 eyeDirection = normalize(uEye.position - vPosition);
    vec3 lightDirection = normalize(-uLight.direction);
    vec3 normal = normalize(vNormal);

	vec3 ambient = uLight.ambient * uColor;
    vec3 diffuse = uLight.diffuse * uColor * max(0.0, dot(normal, lightDirection));
    vec3 specular = uLight.specular * uColor * pow(max(0.0, dot(normalize(lightDirection + eyeDirection), normal)), 8.0);

	fragColor = vec4(ambient + diffuse + specular, 1);
}