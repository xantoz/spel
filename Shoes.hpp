#ifndef SHOES_HPP
#define SHOES_HPP
#include "Equippable.hpp"
class Shoes : public Equippable 
{
public:
    Shoes(const std::string &name, const std::string &description,unsigned weight, const Stats &stats);
};
#endif
