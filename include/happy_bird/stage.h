#pragma once

#include <iostream>
#include <memory>
#include <vector>

class World;
class Object;

enum StageFlag{
	kPlainStage = 0,
	kDungeonStage = 1,
	kDefaultStage = 2
};
class Stage{
 public:

	using ObjectContainer = std::vector<std::shared_ptr<Object> >;
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
};