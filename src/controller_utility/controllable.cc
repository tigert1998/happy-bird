#include "controller_utility/controllerable.h"

void set_controller(Controller *controller_ptr) {
	controller_ptr_ = shared_ptr<Controller>(controller_ptr);
}