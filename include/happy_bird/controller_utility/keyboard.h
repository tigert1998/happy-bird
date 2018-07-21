#pragma once

#include <vector>
#include <functional>
#include <bitset>

class Keyboard {
private:
	static constexpr int kTotal = 350;
	std::bitset<kTotal> key_pressed_;
	std::vector<std::function<void(std::bitset<kTotal>, double)>> yields_;

public:
	using KeyboardState = std::bitset<kTotal>;
	static Keyboard shared;
	Keyboard();
	void Trigger(int key, int action);
	void Elapse(double time) const;
	void Register(std::function<void(KeyboardState, double)> yield);

};