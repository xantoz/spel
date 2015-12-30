#include "Armor.hpp"
#include <iostream>

Armor::Armor(const std::string &name,
             const std::string &description,
             unsigned weight,
             const Stats &stats) :
    Equippable(name, description, weight, stats)
{
}

Armor::~Armor()
{
    std::cerr << "Armor<" << this->getName() << "> destructor" << std::endl;
}
