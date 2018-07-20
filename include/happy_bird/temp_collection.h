#pragma once

#include <list>
#include <memory>

#include "object_wrapper.h"
#include "timer.h"

class TempCollection{
	struct TempWrapper: public ObjectWrapper{
		friend TempCollection;
		float duration_;
		Timer::TimingId timer_;
		std::shared_ptr<Object> data_ref_;
	 public:
	 	TempWrapper(Object* naked, float duration):data_ref_(naked), duration_(duration){
	 		timer_ = Timer::New();
	 	}
	 	std::weak_ptr<Object> get(void){
	 		return static_cast<std::weak_ptr<Object> >(data_ref_);
	 	}
	};
 public:
	using ObjectContainer = std::list<TempWrapper>;
	using ObjectIterator = ObjectContainer::iterator;
 	TempCollection(World* world):world_(world){ }
 	~TempCollection(){ }
 	ObjectIterator begin(void){
 		return objects_.begin();
 	}
 	ObjectIterator end(void){
 		return objects_.end();
 	}
 	void Update(void);
 	void PushBack(Object* object, float duration); // second
 private:
 	World* world_;
	ObjectContainer objects_;
};