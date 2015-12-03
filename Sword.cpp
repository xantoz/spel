#include "Sword.hpp"
Sword::Sword(const std::string &name,
             const std::string &description,
             unsigned weight,
             const Stats &stats) :
    Equippable(name, description, weight, stats)
{
}

