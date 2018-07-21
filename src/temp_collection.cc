#include "temp_collection.h"
#include "object.h"

void TempCollection::Update(void){
	for(auto p = objects_.begin(); p != objects_.end(); ){
		if(Timer::Query(p->timer_) >= p->duration_){
			p = objects_.erase(p);
		}
		else{
			p ++;
		}
	}
}
void TempCollection::PushBack(Object* object, float duration){
	objects_.push_back(TempWrapper(object, duration));
}
void TempCollection::PushBack(std::shared_ptr<Object> object, float duration){
	objects_.push_back(TempWrapper(object, duration));
}