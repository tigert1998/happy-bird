#include "controller_utility/controller.h"

class AutomationController: public Controller {
private:
	Character &target_;

public:
	AutomationController() = delete;
	AutomationController(Character &controlee, Character &target);
	void Elapse(double time) const;
};