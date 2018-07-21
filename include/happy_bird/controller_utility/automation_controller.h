#pragma once

#include "controller_utility/controller.h"
#include "opengl_common.h"

#include <memory>

class AutomationController: public Controller {
private:
	enum class PatrolDirection {
		kXPosition, kXNegative
	};
	PatrolDirection patrol_direction_;
	Character &target_;
	std::shared_ptr<glm::vec3> patrol_ptr_;
	static const float patrol_radius_;

public:
	AutomationController() = delete;
	AutomationController(Character &controlee, Character &target);
	void Elapse(double time);
};