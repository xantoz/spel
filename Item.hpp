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
    virtual ~Item() override;
    
    unsigned getWeight() const;
    ItemOwner *getOwner() const;
    
    virtual std::string use();
    
    friend class ItemOwner;
};

#endif
