#include "player_collection.h"

#include <algorithm>

using std::shared_ptr;
using std::weak_ptr;

PlayerCollection::PlayerCollection() = default;

void PlayerCollection::PushBackFriendly(Object* object) {
	friendly_collection_.emplace_back(object);
}

void PlayerCollection::PushBackHostile(Object* object) {
	hostile_collection_.emplace_back(object);
}

void PlayerCollection::Traverse(std::function<void(std::weak_ptr<Object>)> yield, std::function<bool(Object*, Object*)> compare_function) {
	sort(friendly_collection_.begin(), friendly_collection_.end(), [&] (const shared_ptr<Object>& a, const shared_ptr<Object>& b) -> bool {
		return compare_function(a.get(), b.get());
	});
	sort(hostile_collection_.begin(), hostile_collection_.end(), [&] (const shared_ptr<Object>& a, const shared_ptr<Object>& b) -> bool {
		return compare_function(a.get(), b.get());
	});
	int i = 0, j = 0;
	while (i < friendly_collection_.size() || j < hostile_collection_.size()) {
		if (i == friendly_collection_.size()) {
			yield(hostile_collection_[j++]);
		} else if (j == hostile_collection_.size()) {
			yield(friendly_collection_[i++]);
		} else {
			if (compare_function(friendly_collection_[i].get(), hostile_collection_[j].get())) {
				yield(friendly_collection_[i++]);
			} else {
				yield(hostile_collection_[j++]);
			}
		}
	}
}