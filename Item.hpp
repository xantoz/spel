#ifndef ITEM_HPP
#define ITEM_HPP

// #include "ItemOwner.hpp"
#include "GameObject.hpp"

#include <ostream>

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

    virtual std::string serialize(std::ostream &os) const;
    
    friend class ItemOwner;
};

#endif
