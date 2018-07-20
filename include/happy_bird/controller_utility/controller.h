#pragma once

#include "character.h"

class Controller {
protected:
	Character &controlee_;

public:
	Controller() = delete;
	Controller(Character &controlee);
	virtual void Elapse(double time) const = 0;
};