#include "math_utility.h"
#include "controller_utility/automation_controller.h"
#include "opengl_common.h"

using std::shared_ptr;

const float AutomationController::patrol_radius_ = 10;

AutomationController::AutomationController(Character &controlee, Character &target): target_(target), Controller(controlee) {
	patrol_ptr_ = nullptr;
}

void AutomationController::Elapse(double time) {
	auto controlee_position = BTVector3ToGLMVec3(controlee_.GetDelegate()->GetOrigin());
	auto target_position = BTVector3ToGLMVec3(target_.GetDelegate()->GetOrigin());
	auto distance = glm::distance(controlee_position, target_position);
	if (distance >= 10) {
		if (patrol_ptr_ == nullptr) {
			patrol_ptr_ = shared_ptr<glm::vec3>(new glm::vec3(controlee_position));
			patrol_direction_ = rand() % 2 ? PatrolDirection::kXPosition : PatrolDirection::kXNegative;
		}
		switch (patrol_direction_) {
			case PatrolDirection::kXPosition: {
				if (controlee_position.x >= patrol_ptr_->x + patrol_radius_)
					patrol_direction_ = PatrolDirection::kXNegative;
				break;
			}
			case PatrolDirection::kXNegative: {
				if (controlee_position.x <= patrol_ptr_->x - patrol_radius_)
					patrol_direction_ = PatrolDirection::kXPosition;
				break;
			}
		}
		controlee_.Rotate(patrol_direction_ == PatrolDirection::kXPosition, time);
	} else {
		patrol_ptr_ = nullptr;
		auto delta_x = target_position.x - controlee_position.x;
		auto delta_z = target_position.z - controlee_position.z;
		if (abs(delta_x) > abs(delta_z)) {
			controlee_.Rotate(delta_x > 0, time);
		} else {
			controlee_.Move(delta_z > 0, time);
		}
	}
}