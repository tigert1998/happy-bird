#pragma once

#include <memory>

#include "object.h"
#include "controller_utility/automation_controller.h"
#include "controller_utility/keyboard_controller.h"

class Player {
 private:
	std::shared_ptr<Object> object_ptr_;
	std::shared_ptr<Character> character_ptr_;
	std::shared_ptr<Controller> controller_ptr_;
	bool is_disabled_ = false;

 public:
	Player() = delete;
	Player(Object *object_ptr, Character *character_ptr, Controller *controller_ptr);
	std::weak_ptr<Object> object_ptr() const;
	std::weak_ptr<Character> character_ptr() const;
	std::weak_ptr<Controller> controller_ptr() const;
	bool is_disabled() const;
	void set_is_disabled(bool is_disabled);
	static std::shared_ptr<Player> RandomEnemyPlayer(std::weak_ptr<Character> target_ptr, World *world_ptr);
};