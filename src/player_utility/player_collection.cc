#include "player_utility/player_collection.h"
#include "controller_utility/automation_controller.h"
#include "controller_utility/keyboard_controller.h"
#include "shader_utility/pure_color_material.h"
#include "shader_utility/texture_material.h"

#include <algorithm>

using std::shared_ptr;
using std::weak_ptr;

PlayerCollection::PlayerCollection() = default;

void PlayerCollection::PushBackFriendly(shared_ptr<Player> player_ptr) {
	friendly_collection_.emplace_back(player_ptr);
}

void PlayerCollection::PushBackHostile(shared_ptr<Player> player_ptr) {
	hostile_collection_.emplace_back(player_ptr);
}

void PlayerCollection::Traverse(std::function<void(std::weak_ptr<Player>)> yield, std::function<bool(const Player &, const Player &)> compare_function) {
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

void PlayerCollection::InitPlayerCollection(World *world_ptr) {
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(World::origin + btVector3(0, World::character_height, -World::character_height));

	Object *object_ptr = new Hero(
		world_ptr,
		nullptr,
		new TextureMaterial("resources/hero.png", "resources/hero.png", 8),
		transform,
		World::character_height,
		World::character_height
	);
	Character *character_ptr = new CharacterImpl(world_ptr, object_ptr);
	Controller *controller_ptr = new KeyboardController(*character_ptr);
	world_ptr->camera->set_accompany_object(object_ptr, 120);

	auto particle_ptr = new Particle(
		world_ptr,
		nullptr,
		new PureColorMaterial(color::Red(), color::Red(), 40),
		btVector3(0, 0, 0),
		glm::vec3(0, 0, -0.02),
		kLargeParticle | kFlameParticle | kAmbientParticle
	);
	particle_ptr->Attach(object_ptr, btVector3(0, 3, 0));

	auto main_player_ptr = shared_ptr<Player>(new Player(object_ptr, character_ptr, controller_ptr));
	PushBackFriendly(main_player_ptr);

	for (int i = 0; i < 3; i++) {
		PushBackHostile(Player::RandomEnemyPlayer(main_player_ptr->character_ptr(), world_ptr));
	}
}