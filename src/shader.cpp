#include "shader.h"

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
void Shader::SetUniform(const std::string &identifier, btTransform value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	btScalar data[16];
	value.getOpenGLMatrix(data);
	glUniformMatrix4fv(location, 1, GL_FALSE, data);
}

template <> 
void Shader::SetUniform(const std::string &identifier, glm::vec3 value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniform3fv(location, 1, value_ptr(value));
}

template <> 
void Shader::SetUniform(const std::string &identifier, glm::mat4 value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

template <> 
void Shader::SetUniform(const std::string &identifier, int32_t value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniform1i(location, value);
}

template <> 
void Shader::SetUniform(const std::string &identifier, float value) const {
	auto location = glGetUniformLocation(id, identifier.c_str());
#ifdef DEBUG
	if (location < 0) throw ShaderSettingError(identifier);	
#endif
	glUniform1f(location, value);
}