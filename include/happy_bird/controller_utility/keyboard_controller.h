#pragma once

#include "controller_utility/controller.h"
#include "controller_utility/keyboard.h"

class KeyboardController: public Controller {
public:
	KeyboardController() = delete;
	KeyboardController(Character &controlee, Keyboard &keyboard = Keyboard::shared);
};