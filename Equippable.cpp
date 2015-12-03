#include "Equippable.hpp"
Equippable::Equippable(const std::string &name, const std::string &description, unsigned weight, const Stats &s) :
    Item(name, description, weight), stats(s)
{
}

const Stats &Equippable::getStats() const
{
    return stats;
}

