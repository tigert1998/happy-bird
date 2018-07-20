#pragma once

#include "controller_utility/controllable.h"
#include "controller_utility/controller.h"

class KeyboardController: public Controller {
public:
	KeyboardController() = delete;
	KeyboardController(Controllable &controlee, Keyboard &keyboard = Keyboard::shared);
};