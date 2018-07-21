#include <algorithm>
#include <memory>
#include <iostream>

using std::shared_ptr;
using std::weak_ptr;
using std::max;
using std::min;
using std::cout;
using std::endl;
using std::boolalpha;
using std::function;
using std::vector;

#include "player_utility/player_collection.h"
#include "controller_utility/automation_controller.h"
#include "controller_utility/keyboard_controller.h"
#include "shader_utility/pure_color_material.h"
#include "shader_utility/texture_material.h"
#include "math_utility.h"


PlayerCollection::PlayerCollection() = default;

void PlayerCollection::PushBackFriendly(shared_ptr<Player> player_ptr) {
	friendly_collection_.emplace_back(player_ptr);
}

void PlayerCollection::PushBackHostile(shared_ptr<Player> player_ptr) {
	hostile_collection_.emplace_back(player_ptr);
}

void PlayerCollection::Traverse(function<void(weak_ptr<Player>)> yield, function<bool(const Player &, const Player &)> compare_function) {
	vector<shared_ptr<Player>> new_hostile_collection;
	for_each(hostile_collection_.begin(), hostile_collection_.end(), [&](shared_ptr<Player> player_ptr) {
		if (!player_ptr->is_disabled())
			new_hostile_collection.push_back(player_ptr);
	});
	hostile_collection_ = new_hostile_collection;
	sort(friendly_collection_.begin(), friendly_collection_.end(), [&] (const shared_ptr<Player> &a, const shared_ptr<Player> &b) -> bool {
		return compare_function(*a, *b);
	});
	sort(hostile_collection_.begin(), hostile_collection_.end(), [&] (const shared_ptr<Player> &a, const shared_ptr<Player> &b) -> bool {
		return compare_function(*a, *b);
	});
	int i = 0, j = 0;
	while (i < friendly_collection_.size() || j < hostile_collection_.size()) {
		if (i == friendly_collection_.size()) {
			yield(hostile_collection_[j++]);
		} else if (j == hostile_collection_.size()) {
			yield(friendly_collection_[i++]);
		} else {
			if (compare_function(*friendly_collection_[i], *hostile_collection_[j])) {
				yield(friendly_collection_[i++]);
			} else {
				yield(hostile_collection_[j++]);
			}
		}
	}
}

void PlayerCollection::Query(glm::vec3 location, float delta_x, float delta_z, std::function<void(std::weak_ptr<Player>)> yield) {
	delta_z = fabs(delta_z);
	for (auto player_ptr : friendly_collection_) {
		auto player_location = BTVector3ToGLMVec3(player_ptr->object_ptr().lock()->GetOrigin());
		if (!(location.x + min(delta_x, 0.0f) <= player_location.x && player_location.x <= location.x + max(delta_x, 0.0f)))
			continue;
		if (!(location.z + delta_z / 2.0 >= player_location.z && location.z - delta_z / 2.0 <= player_location.z))
			continue;
		yield(player_ptr);
	}

	for (auto player_ptr : hostile_collection_) {
		auto player_location = BTVector3ToGLMVec3(player_ptr->object_ptr().lock()->GetOrigin());
		if (!(location.x + min(delta_x, 0.0f) <= player_location.x && player_location.x <= location.x + max(delta_x, 0.0f)))
			continue;
		if (!(location.z + delta_z / 2.0 >= player_location.z && location.z - delta_z / 2.0 <= player_location.z))
			continue;
		yield(player_ptr);
	}
}

void PlayerCollection::InitPlayerCollection(World *world_ptr) {
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(World::origin + btVector3(45, 40, 30));

	Object *object_ptr = new Hero(
		world_ptr,
		nullptr,
		new TextureMaterial("resources/hero.png", "resources/hero.png", 8),
		transform,
		World::character_height,
		World::character_height
	);
	Character *character_ptr = new Character(world_ptr, object_ptr);
	Controller *controller_ptr = new KeyboardController(*character_ptr);
	world_ptr->camera->set_accompany_object(object_ptr, 120);

	auto main_player_ptr = shared_ptr<Player>(new Player(object_ptr, character_ptr, controller_ptr));
	PushBackFriendly(main_player_ptr);

	for (int i = 0; i < 3; i++) {
		PushBackHostile(Player::RandomEnemyPlayer(main_player_ptr->character_ptr(), world_ptr));
	}
}