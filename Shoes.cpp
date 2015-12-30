#include "Shoes.hpp"
#include <iostream>

Shoes::Shoes(const std::string &name,
             const std::string &description,
             unsigned weight,
             const Stats &stats)
    : Equippable(name, description, weight, stats)
{
}


Shoes::~Shoes()
{
    std::cerr << "Shoes<" << this->getName() << "> destructor" << std::endl;
}
