#pragma once

class Controller{
 public: 	
 private:
	std::shared_ptr<Arena> arena_;
	Binder binder_;
	TimedAllocator timer_;
	AutomationController auto_ctrl_;
	KeyboardController key_ctrl_;
};