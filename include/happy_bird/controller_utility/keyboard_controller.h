#pragma once

#include "controller_utility/controller.h"
#include "controller_utility/keyboard.h"

class KeyboardController: public Controller {
private:
	Keyboard &keyboard_;
	
public:
	KeyboardController() = delete;
	KeyboardController(Character &controlee, Keyboard &keyboard = Keyboard::shared);
	void Elapse(double time) const;
};