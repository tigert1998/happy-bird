#pragma once

#include "object_common.h"

#include <vector>
#include <memory>
#include <functional>

class PlayerCollection {
private:
	std::vector<std::shared_ptr<Object>> hostile_collection_, friendly_collection_;

public:
	PlayerCollection();
	void PushBackHostile(Object* object);
	void PushBackFriendly(Object* object);
	void Traverse(
		std::function<void(std::weak_ptr<Object>)> yield,
		std::function<bool(Object*, Object*)> compare_function = [] (Object* a, Object* b) {
			return a->GetOrigin()[2] > b->GetOrigin()[2];
		}
	);
};