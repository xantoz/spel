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

    Item(std::string name, std::string description, unsigned weight);
    
    const std::string& getName() const;
    const std::string& getDescription() const;
    unsigned getWeight() const;

    friend class ItemOwner;
};

#endif
