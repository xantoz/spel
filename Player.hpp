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
    virtual bool can_carry(const Item *i) const override;
public:
    
};
#endif
