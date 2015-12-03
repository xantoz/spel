#ifndef EQUIPPABLE_HPP
#define EQUIPPABLE_HPP

#include "Item.hpp"
#include "Stats.hpp"

class Equippable : public Item 
{
private:
    const Stats stats;

public:
    Equippable(const std::string &name, const std::string &description, unsigned weight, const Stats &s);

    const Stats &getStats() const;
};
#endif
