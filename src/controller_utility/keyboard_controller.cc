#include "opengl_common.h"
#include "controller_utility/keyboard_controller.h"
#include "controller_utility/character.h"

#include <functional>
#include <iostream>

using std::bind;

KeyboardController::KeyboardController(Character &controlee, Keyboard &keyboard): Controller(controlee), keyboard_(keyboard) {
	keyboard.Register([&] (Keyboard::KeyboardState state, double time) {
		if (state[GLFW_KEY_W])
			controlee_.Move(true, time);
		else if (state[GLFW_KEY_S])
			controlee_.Move(false, time);
		else
			controlee_.ResetMove();
	
		if (state[GLFW_KEY_A])
			controlee_.Rotate(true, time);
		else if (state[GLFW_KEY_D])
			controlee_.Rotate(false, time);
		else
			controlee_.ResetRotate();

		if (state[GLFW_KEY_F]){
			std::cout << "Press F" << std::endl;
			controlee_.LaserAttack();
		}
		if (state[GLFW_KEY_G]){
			std::cout << "Press G" << std::endl;
			controlee_.BoxAttack();
		}
		if (state[GLFW_KEY_SPACE])
			controlee_.Jump(time);
	});
}

void KeyboardController::Elapse(double time) {
	keyboard_.Elapse(time);
}