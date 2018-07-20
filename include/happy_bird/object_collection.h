#pragma once

#include <memory>

class ObjectCollection{
 public:
	using ObjectPtr = std::shared_ptr<Object>;
	using ObjectRef = std::weak_ptr<Object>;


	ObjectRef reset(void);
	ObjectRef next(void);
	ObjectRef is_end(void);
};