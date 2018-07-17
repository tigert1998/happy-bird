#include <iostream>
using std::cout;
using std::endl;

#include "shader_utility/shader.h"
#include "shader_utility/material.h"
#include "shader_utility/parallel_light.h"
#include "shader_utility/point_light.h"
#include "shader_utility/light_collection.h"

Shader::Shader(const std::string &vs_path, const std::string &fs_path) {
	auto vs_source = file_manager.FileContentAt(vs_path);
	auto fs_source = file_manager.FileContentAt(fs_path);
	auto vs_id = Compile(GL_VERTEX_SHADER, vs_source, vs_path);
	auto fs_id = Compile(GL_FRAGMENT_SHADER, fs_source, fs_path);
	id = Link(vs_id, fs_id);
}

uint32_t Shader::Compile(GLenum type, const std::string &source, const std::string &path) {
	uint32_t shader_id = glCreateShader(type);
	const char *temp = source.c_str();
	glShaderSource(shader_id, 1, &temp, nullptr);
	glCompileShader(shader_id);
	int success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (success) return shader_id;
	int length;
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
	char *log = new char[length];
	glGetShaderInfoLog(shader_id, length, nullptr, log);
	std::string log_str = log;
	delete [] log;
	throw ShaderCompileError(path, log_str);
}

uint32_t Shader::Link(uint32_t vs_id, uint32_t fs_id) {
	uint32_t program_id = glCreateProgram();
	glAttachShader(program_id, vs_id);
	glAttachShader(program_id, fs_id);
	glLinkProgram(program_id);
	glDeleteShader(vs_id);
	glDeleteShader(fs_id);
	int success;
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (success) return program_id;
	int length;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
	char *log = new char[length];
	glGetProgramInfoLog(program_id, length, nullptr, log);
	std::string log_str = log;
	delete [] log;
	throw ShaderLinkError(log_str);
}

void Shader::Use() const {
	glUseProgram(id);
}

template <>
void Shader::SetUniform<btTransform>(const std::string &identifier, const btTransform& value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	btScalar data[16];
	value.getOpenGLMatrix(data);
	glUniformMatrix4fv(location, 1, GL_FALSE, data);
}

template <> 
void Shader::SetUniform<glm::vec3>(const std::string &identifier, const glm::vec3& value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniform3fv(location, 1, value_ptr(value));
}

template <> 
void Shader::SetUniform<glm::mat4>(const std::string &identifier, const glm::mat4& value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

template <> 
void Shader::SetUniform<int32_t>(const std::string &identifier, const int32_t& value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniform1i(location, value);
}

template <> 
void Shader::SetUniform<float>(const std::string &identifier, const float& value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniform1f(location, value);
}

template <>
void Shader::SetUniform(const std::string &identifier, const Attenuation& value) const {
	SetUniform<float>(identifier + ".range", value.range());
	SetUniform<float>(identifier + ".constant", value.constant());
	SetUniform<float>(identifier + ".linear", value.linear());
	SetUniform<float>(identifier + ".quadratic", value.quadratic());
}

template <>
void Shader::SetUniform<Material>(const std::string &identifier, const Material& value) const {
	SetUniform<glm::vec3>(identifier + ".diffuse", value.diffuse());
	SetUniform<glm::vec3>(identifier + ".specular", value.specular());
	SetUniform<float>(identifier + ".shininess", value.shininess());
}

template <>
void Shader::SetUniform(const std::string &identifier, const Light& value) const {
	SetUniform<glm::vec3>(identifier + ".color", value.color());
	SetUniform<float>(identifier + ".intensity", value.intensity());
	switch (value.type()) {
		case LightType::kPoint:
			SetUniform<glm::vec3>(identifier + ".position", dynamic_cast<const PointLight&>(value).position());
			SetUniform<Attenuation>(identifier + ".attenuation", dynamic_cast<const PointLight&>(value).attenuation());
			break;
		case LightType::kParallel:
			SetUniform<glm::vec3>(identifier + ".direction", dynamic_cast<const ParallelLight&>(value).direction());
			break;
	}
}

template <>
void Shader::SetUniform<LightCollection>(const std::string &identifier, const LightCollection& value) const {
	SetUniform<glm::vec3>(identifier + ".ambient", value.ambient());
	int point_light_total = 0, parallel_light_total = 0;
	for (int i = 0; i < value.Size(); i++) {
		std::string sub_identifier;
		switch (value[i]->type()) {
			case LightType::kPoint:
				sub_identifier = identifier + ".point[" + std::to_string(point_light_total++) + "]";
				break;
			case LightType::kParallel:
				sub_identifier = identifier + ".parallel[" + std::to_string(parallel_light_total++) + "]";
				break;
		}
		SetUniform<Light>(sub_identifier, *value[i]);
	}
}