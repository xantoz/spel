#ifndef EQUIPPABLE_HPP
#define EQUIPPABLE_HPP

#include "Item.hpp"
#include "Stats.hpp"

class Equippable : public Item 
{
public:
    Equippable(const std::string &name, const std::string &description, const Stats &s);
    
private:
    const Stats stats;
    
};
#endif
