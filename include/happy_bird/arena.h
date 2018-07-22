#pragma once

#include <cstdint>

class Arena{
 public:
	using ObjectPtr = std::shared_ptr<Object>;
	using ObjectRef = std::weak_ptr<Object>;
	using ObjectId = uint32_t;
	ObjectId NewObject(const ObjectConfig&);
	ObjectRef GetObject(ObjectId);
	bool ImportToBulletModel(ObjectId);
	bool ExportFromBulletModel(ObjectId);
	bool DeleteObject(ObjectId);
 private:
	// stage
	std::vector<ObjectPtr> static_;
	// model controlled, need sorting
	std::vector<ObjectPtr> heap_;
	// volatile
	std::vector<ObjectPtr> stack_;

};