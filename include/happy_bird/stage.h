#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "object_wrapper.h"

class World;
class Object;

enum StageFlag{
	kPlainStage = 0,
	kDungeonStage = 1,
	kDefaultStage = 2
};
class Stage{
	struct StageWrapper: public ObjectWrapper{
		friend Stage;
		std::shared_ptr<Object> data_ref_;
	 public:
	 	StageWrapper(Object* naked): data_ref_(naked){ }
	 	std::weak_ptr<Object> get(void){
	 		return static_cast<std::weak_ptr<Object> >(data_ref_);
	 	}
	};
 public:
	using ObjectContainer = std::vector<StageWrapper>;
	using ObjectIterator = ObjectContainer::iterator;
	Stage(World* world):world_(world){ }
	~Stage(){ }
	ObjectIterator begin(void){
		return objects_.begin();
	}
	ObjectIterator end(void){
		return objects_.end();
	}
	void InitStage(StageFlag);
 private:
	World* world_;
	ObjectContainer objects_;
	void InitDungeonStage(void);
	void InitDefaultStage(void);
	// void NewStair()
};