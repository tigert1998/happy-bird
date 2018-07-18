#pragma once
#include <algorithm>

#include "opengl_common.h"
#include "bullet_common.h"
#include "math_utility.h"
#include "object.h"
#include "head.h"
#include "character.h"

class Camera {
public:
	enum MoveDirectionType{
		kFront = 0,
		kBack = 1,
		kLeft = 2,
		kRight = 3
	};
	Camera() = delete;
	Camera(glm::vec3 position, double width_height_ratio, double alpha = -glm::pi<double>() / 2, double beta = 0);
	void Rotate(double delta_alpha, double delta_beta);
	void Move(MoveDirectionType, float time);
	glm::vec3 position() const;
	void set_position(glm::vec3);
	glm::mat4 view_matrix() const;
	void set_width_height_ratio(double width_height_ratio);
	glm::mat4 projection_matrix() const;
	double alpha() const;
	double beta() const;
	void set_alpha(double alpha);
	void set_beta(double beta);
	glm::vec3 front() const;
	void set_front(glm::vec3 new_front);
	glm::vec3 center() const;
	void set_center(glm::vec3 new_center);
	void set_accompany_object(Object* accompany_object, double surround_radius);

private:
	static const double kMaxElevation;
	const glm::vec3 up_ = glm::vec3(0, 1, 0);
	glm::vec3 position_;
	double alpha_, beta_, width_height_ratio_, surround_radius_;
	Object* accompany_object_;
	
};