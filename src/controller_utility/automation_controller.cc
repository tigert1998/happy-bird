#include "math_utility.h"
#include "controller_utility/automation_controller.h"
#include "opengl_common.h"

AutomationController::AutomationController(Character &controlee, Character &target): target_(target), Controller(controlee) { }

void AutomationController::Elapse(double time) const {
	auto controlee_position = BTVector3ToGLMVec3(controlee_.GetDelegate()->GetOrigin());
	auto target_position = BTVector3ToGLMVec3(target_.GetDelegate()->GetOrigin());
	auto distance = glm::distance(controlee_position, target_position);
	if (distance >= 10) return;
	auto delta_x = target_position.x - controlee_position.x;
	auto delta_z = target_position.z - controlee_position.z;
	if (abs(delta_x) > abs(delta_z)) {
		controlee_.Rotate(delta_x > 0, time);
	} else {
		controlee_.Move(delta_z > 0, time);
	}
}