#pragma once

struct vector_converter{
	static btVector3 ToBtVec(glm::vec3 vec){
		return btVector3(vec[0],vec[1],vec[2]);
	}	
};
