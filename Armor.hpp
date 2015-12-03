#ifndef ARMOR_HPP
#define ARMOR_HPP
#include "Equippable.hpp"
class Armor : public Equippable 
{
public:
    Armor(const std::string &name, const std::string &description,unsigned weight, const Stats &stats);
    
    
};
#endif
