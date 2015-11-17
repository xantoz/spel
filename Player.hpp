#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Actor.hpp"
#include <stdlib>
#include <iostream>
class Player : public Actor
{
private:
    unsigned strength;
    static unsigned magisk_konstant = 10;
protected:
    virtual bool can_carry(const Item *i) const override
    {
        unsigned total_weight = 0;
        for(Item *i: items)
            total_weight += i->getWeight();
        total_weight += i->getWeight();
        return (total_weight > strength*magisk_konstant);
    }

public:
    
};
#endif
