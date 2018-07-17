#pragma once

#include <glm/glm.hpp>

typedef glm::vec3 Color;

struct color{
	static Color Black(void){
		return glm::vec3(0,0,0);
	}
	static Color White(void){
		return glm::vec3(1,1,1);
	}
	static Color Red(void){
		return glm::vec3(0.956863,0.262745,0.211765);
	}
	static Color Yellow(void){
		return glm::vec3(1,0.921569,0.231373);
	}
	static Color Green(void){
		return glm::vec3(0.803922,0.862745,0.223529);
	}
	static Color Purple(void){
		return glm::vec3(0.611765,0.152941,0.690196);
	}
};


