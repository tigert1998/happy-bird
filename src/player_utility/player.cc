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
	static std::uniform_real_distribution<> width_distribution(10, 260); 
	static std::uniform_real_distribution<> depth_distribution(10, 30);
	static std::uniform_real_distribution<> speed_distribution(7, 13);
	static std::uniform_real_distribution<> radius_distribution(5, 20);

	static auto width_dice = std::bind(width_distribution, engine);
	static auto depth_dice = std::bind(depth_distribution, engine);
	static auto speed_dice = std::bind(speed_distribution, engine);
	static auto radius_dice = std::bind(radius_distribution, engine);

	auto texture_path = "resources/enemies/" + to_string(rand() % PNG_TOTAL) + ".png";
	
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(World::origin + btVector3(width_dice(), World::character_height, depth_dice()));
	auto object_ptr = new Hero(
		world_ptr,
		nullptr,
		new TextureMaterial(texture_path, texture_path, 8),
		transform,
		World::character_height,
		World::character_height
	);

	auto character_ptr = new Character(world_ptr, object_ptr);
	character_ptr->set_max_speed(speed_dice());

	auto controller_ptr = new AutomationController(*character_ptr, *target_ptr.lock(), radius_dice());

	return shared_ptr<Player>(new Player(object_ptr, character_ptr, controller_ptr));
}