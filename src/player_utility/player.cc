#include "world.h"
#include "player_utility/player.h"
#include "shader_utility/texture_material.h"

#include <random>

using std::shared_ptr;
using std::weak_ptr;
using std::to_string;

Player::Player(Object *object_ptr, Character *character_ptr, Controller *controller_ptr) {
	object_ptr_ = shared_ptr<Object>(object_ptr);
	character_ptr_ = shared_ptr<Character>(character_ptr);
	controller_ptr_ = shared_ptr<Controller>(controller_ptr); 
}

weak_ptr<Object> Player::object_ptr() const {
	return object_ptr_;
}

weak_ptr<Character> Player::character_ptr() const {
	return character_ptr_;
}

weak_ptr<Controller> Player::controller_ptr() const {
	return controller_ptr_;
}

shared_ptr<Player> Player::RandomEnemyPlayer(weak_ptr<Character> target_ptr, World *world_ptr) {
	static constexpr int PNG_TOTAL = 13;
	static std::default_random_engine engine(time(nullptr));
	static std::uniform_real_distribution<> distribution(-5 * World::character_height, 5 * World::character_height); 
	static auto dice = std::bind(distribution, engine);
	
	auto texture_path = "resources/enemies/" + to_string(rand() % PNG_TOTAL) + ".png";
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(World::origin + btVector3(dice(), World::character_height, dice()));
	auto object_ptr = new Hero(
		world_ptr,
		nullptr,
		new TextureMaterial(texture_path, texture_path, 8),
		transform,
		World::character_height,
		World::character_height
	);

	auto character_ptr = new CharacterImpl(world_ptr, object_ptr);
	auto controller_ptr = new AutomationController(*character_ptr, *target_ptr.lock());

	return shared_ptr<Player>(new Player(object_ptr, character_ptr, controller_ptr));
}