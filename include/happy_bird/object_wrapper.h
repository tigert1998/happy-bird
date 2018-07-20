#pragma once

#include <memory>

#include "object.h"

class ObjectWrapper{
 public:
 	virtual std::weak_ptr<Object> get(void) = 0;
};