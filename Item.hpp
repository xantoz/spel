#ifndef ITEM_HPP
#define ITEM_HPP

// #include "ItemOwner.hpp"
#include "GameObject.hpp"

class ItemOwner;                                            // solve circular reference
class Item : public GameObject
{
private:
    unsigned weight;
    ItemOwner *owner;
public:

    Item(std::string name, std::string description, unsigned weight);
    Item(std::string name, std::string description, unsigned weight, ItemOwner *owner);
    virtual ~Item() override;
    
    unsigned getWeight() const;
    ItemOwner *getOwner() const;
    
    virtual std::string use();
    
    friend class ItemOwner;
    friend class Actor; // needed to fipple ownership of items as they are equipped
};

#endif
