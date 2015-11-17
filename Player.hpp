#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Actor.hpp"
#include "Item.hpp"
#include <iostream>

class Player : public Actor
{
private:
    unsigned strength;
    const static unsigned magisk_konstant = 10;
protected:
    virtual bool can_carry(const Item *i) const override;
public:
    
};

#endif
