#ifndef ITEM_HPP
#define ITEM_HPP

#include "ItemOwner.hpp"

class Item
{
private:
    ItemOwner *owner;
    std::string name;
    std::string description;
    unsigned weight;
public:

    Item(std::string nam, std::string descriptio, unsigned weigh) : owner(nullptr), name(nam), description(descriptio), weight(weigh)
    {
    }

    const std::string& getName() const
    {
        return name;
    }

    const std::string& getDescription() const
    {
        return description;
    }

    unsigned getWeight() const
    {
        return weight;
    }

    friend class ItemOwner;
};

#endif
