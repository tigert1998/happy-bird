#pragma once

class PlayerCollection;

#include "object_common.h"
#include "player_utility/player.h"

#include <vector>
#include <memory>
#include <functional>

class PlayerCollection {
private:
	std::vector<std::shared_ptr<Player>> hostile_collection_, friendly_collection_;

public:
	PlayerCollection();
	void PushBackHostile(std::shared_ptr<Player> player_ptr);
	void PushBackFriendly(std::shared_ptr<Player> player_ptr);
	void Traverse(
		std::function<void(std::weak_ptr<Player>)> yield,
		std::function<bool(const Player &, const Player &)> compare_function = [] (const Player &a, const Player &b) -> bool {
			return a.object_ptr().lock()->GetOrigin()[2] > b.object_ptr().lock()->GetOrigin()[2];
		}
	);
	void Query(glm::vec3 location, float width, float depth, std::function<void(std::weak_ptr<Player>)> yield);
	void InitPlayerCollection(World *world_ptr);
	std::shared_ptr<Player> leader(void){
		if(friendly_collection_.size() <= 0)return nullptr;
		return friendly_collection_[0];
	}
};