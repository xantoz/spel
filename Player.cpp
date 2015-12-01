#include "Player.hpp"
#include <iostream>

Player::Player(const std::string &name, const std::string &description) :
    Actor(name, description)
{
}

Player::~Player()
{
    std::cerr << "Player destructor" << std::endl;
}

bool Player::can_carry(const Item *i) const
{
	unsigned total_weight = 0;
    for(Item *i: items)
    	total_weight += i->getWeight();
    
    return (total_weight > strength*magisk_konstant);
}




