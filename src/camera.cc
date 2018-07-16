#include "camera.h"

const double Camera::kMaxElevation = 5 * glm::pi<double>() / 12;

glm::mat4 Camera::projection_matrix() const {
	using namespace glm;
	return perspective(radians(45.0f), 1.0f * (float) width_height_ratio_, 0.1f, 1000.0f);
}

void Camera::set_width_height_ratio(double width_height_ratio) {
	width_height_ratio_ = width_height_ratio;
}

Camera::Camera(glm::vec3 position, double width_height_ratio, double alpha, double beta) {
	position_ = position;
	alpha_ = alpha;
	beta_ = beta;
	width_height_ratio_ = width_height_ratio;
	accompany_object_ = nullptr;
}

double Camera::alpha() const {
	return alpha_;
}

double Camera::beta() const {
	return beta_;
}

void Camera::set_alpha(double alpha) {
	alpha_ = alpha;
}

void Camera::set_beta(double beta) {
	beta_ = beta;
}

void Camera::Rotate(double delta_alpha, double delta_beta) {
	alpha_ += delta_alpha;
	beta_ += delta_beta;
	beta_ = std::min(kMaxElevation, beta_);
	beta_ = std::max(-kMaxElevation, beta_);
}

glm::vec3 Camera::position() const {
	if (accompany_object_ != nullptr) {
		auto origin = BTVector3ToGLMVec3(accompany_object_->GetOrigin());
		return origin - front() * (float) surround_radius_;
	} else {
		return position_;
	}
}

void Camera::set_position(glm::vec3 position) {
	position_ = position;
}

glm::vec3 Camera::front() const {
	return glm::vec3(cos(alpha_) * cos(beta_), sin(beta_), sin(alpha_) * cos(beta_));
}

void Camera::set_front(glm::vec3 new_front) {
	new_front = glm::normalize(new_front);
	beta_ = asin(new_front.y);
	alpha_ = acos(new_front.x / pow(pow(new_front.x, 2) + pow(new_front.z, 2), 0.5));
	if (new_front.z < 0) alpha_ = 2 * glm::pi<double>() - alpha_;
}

void Camera::Move(MoveDirectionType direction, float time) {
	auto left = glm::cross(up_, front());
	auto right = -left;
	switch (direction) {
		case MoveDirectionType::kFront:
			position_ += front() * time;
			break;
		case MoveDirectionType::kBack:
			position_ -= front() * time;
			break;
		case MoveDirectionType::kLeft:
			position_ += left * time;
			break;
		case MoveDirectionType::kRight:
			position_ += right * time;
			break;
	}
}

glm::mat4 Camera::view_matrix() const {
	return glm::lookAt(position(), position() + front(), up_);
}

glm::vec3 Camera::center() const {
	return position() + front();
}

void Camera::set_center(glm::vec3 new_center) {
	auto new_front = new_center - position_;
	set_front(new_front);
}

void Camera::set_accompany_object(Object* accompany_object, double surround_radius) {
	accompany_object_ = accompany_object;
	surround_radius_ = surround_radius;
}