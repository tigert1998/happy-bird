#pragma once

#include <chrono>
#include <cstdint>
#include <vector>

class Timer{
	std::vector<std::chrono::time_point<std::chrono::high_resolution_clock> > pined_;
 public:
 	using TimingId = uint32_t;
 	uint32_t New(void){ // auto pin
 		pined_.push_back(std::chrono::high_resolution_clock::now());
 		return pined_.size() - 1;
 	}
	void Pin(uint32_t id){
		if(id >= pined_.size())return ; // should throw
		pined_[id] = std::chrono::high_resolution_clock::now();
	}
	float Query(uint32_t id) const {
		if(id >= pined_.size())return -1;
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = now - pined_[id];
		return elapsed.count();
	}
	float QueryAndPin(uint32_t id){
		if(id >= pined_.size())return -1;
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = now - pined_[id];
		pined_[id] = now;
		return elapsed.count();
	}
};

static Timer kSharedTimer;