#pragma once

#include "controller_utility/controllable.h"

class Controller {
protected:
	Controllable &controlee_;

public:
	Controller() = delete;
	Controller(Controllable &controlee);
};