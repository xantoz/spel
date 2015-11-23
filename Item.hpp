#ifndef ITEM_HPP
#define ITEM_HPP

// #include "ItemOwner.hpp"
#include "GameObject.hpp"

class ItemOwner;                                            // solve circular reference
class Item : public GameObject
{
private:
    ItemOwner *owner;
    unsigned weight;
public:

    Item(std::string name, std::string description, unsigned weight);
    unsigned getWeight() const;

    virtual std::string use();

    friend class ItemOwner;
};

#endif
