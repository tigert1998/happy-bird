#include "opengl_common.h"
#include "controller_utility/keyboard.h"

Keyboard Keyboard::shared = Keyboard();

Keyboard::Keyboard() = default;

void Keyboard::Trigger(int key, int action) {
	key_pressed_[key] = (action != GLFW_RELEASE);
}

void Keyboard::Elapse(double time) const {
	for (auto f : yields_)
		f(key_pressed_, time);
}

void Keyboard::Register(std::function<void(KeyboardState, double)> yield) {
	yields_.push_back(yield);
}