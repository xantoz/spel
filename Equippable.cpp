#include "Equippable.hpp"
#include <iostream>

Equippable::Equippable(const std::string &name, const std::string &description, unsigned weight, const Stats &s) :
    Item(name, description, weight), stats(s)
{
}

Equippable::~Equippable()
{
    std::cerr << "Equippable<" << this->getName() << "> destructor" << std::endl;
}


const Stats &Equippable::getStats() const
{
    return stats;
}

