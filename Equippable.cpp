#include "Equippable.hpp"
Equippable::Equippable(const std::string &name, const std::string &description, const Stats &s) :
    Item(name, description), stats(s)
{
}
