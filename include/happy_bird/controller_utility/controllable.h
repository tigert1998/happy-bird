#pragma once

#include "controller_utility/controller.h"

#include <memory>

class Controllable {
private:
	std::shared_ptr<Controller> controller_ptr_;

public:
	virtual void MoveLongitudinally(bool is_forward, double time) = 0;
	virtual void MoveLatitudinally(bool is_left, double time) = 0;
	virtual void Jump(double time) = 0;
	virtual void ResetLongitude() = 0;
	virtual void ResetLatitude() = 0;
	void set_controller(Controller *controller_ptr);
};