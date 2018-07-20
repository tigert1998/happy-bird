#include "controller_utility/keyboard_controller.h"
#include "opengl_common.h"

#include <functional>

using std::bind;

KeyboardController::KeyboardController(Controllable &controlee, Keyboard &keyboard): Controller(controlee) {
	Keyboard.Register([&] (Keyboard::KeyboardState state, double time) {
		if (state[GLFW_KEY_W])
			controlee_.MoveLongitudinally(true, time);
		else if (state[GLFW_KEY_S])
			controlee_.MoveLongitudinally(false, time);
		else
			controlee_->ResetLongitude();
	
		if (state[GLFW_KEY_A])
			controlee_.MoveLatitudinally(true, time);
		else if (state[GLFW_KEY_D])
			controlee_.MoveLatitudinally(false, time);
		else
			controlee_.ResetLatitude();
	
		if (state[GLFW_KEY_SPACE])
			controlee_.Jump(time);
	});
}
