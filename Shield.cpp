#include "Shield.hpp"
#include <iostream>

Shield::Shield(const std::string &name,
               const std::string &description,
               unsigned weight,
               const Stats &stats) :
    Equippable(name, description, weight, stats)
{
}

Shield::~Shield()
{
    std::cerr << "Shield<" << this->getName() << "> destructor" << std::endl;
}
