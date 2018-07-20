#version 330 core
struct PureColorMaterial {
	float shininess;
	vec3 diffuse;
	vec3 specular;
};

uniform PureColorMaterial uMaterial;

out vec4 fragColor;

void main() {
	fragColor = vec4(uMaterial.diffuse, 1);
}