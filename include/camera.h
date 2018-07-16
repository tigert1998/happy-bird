#pragma once
#include "opengl_common.h"
#include "bullet_common.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

enum class MoveDirectionType {
	FRONT, BACK, LEFT, RIGHT
};

class Camera {
public:
	Camera() = delete;
	Camera(glm::vec3 position, double width_height_ratio, double alpha = -glm::pi<double>() / 2, double beta = 0);
	void Rotate(double delta_alpha, double delta_beta);
	void Move(MoveDirectionType, float time);
	glm::vec3 position() const;
	void set_position(glm::vec3);
	glm::mat4 GetViewMatrix() const;
	void set_width_height_ratio(double width_height_ratio);
	glm::mat4 GetProjectionMatrix() const;
	double alpha() const;
	double beta() const;
	void set_alpha(double alpha);
	void set_beta(double beta);
	glm::vec3 front() const;
	void set_front(glm::vec3 new_front);
	glm::vec3 center() const;
	void set_center(glm::vec3 new_center);

private:
	const glm::vec3 up_ = glm::vec3(0, 1, 0);
	glm::vec3 position_;
	double alpha_, beta_, width_height_ratio_;
	static const double MAX_ELEVATION;
	
};