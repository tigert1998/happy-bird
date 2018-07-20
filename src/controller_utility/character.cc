#include <iostream>
using std::cout;
using std::endl;

#include "controller_utility/character.h"
#include "world.h"

float Character::static_pace_ = 100;

Character::Character(float speed): max_speed_(speed) { }

Character::~Character() { }

