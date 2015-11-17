#include "Player.hpp"

bool Player::can_carry(const Item *i) const
{
	unsigned total_weight = 0;
    for(Item *i: items)
    	total_weight += i->getWeight();
    
    return (total_weight > strength*magisk_konstant);
}