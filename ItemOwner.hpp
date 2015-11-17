#ifndef ITEMOWNER_HPP
#define ITEMOWNER_HPP

#include "Item.hpp"
#include "GameObject.hpp"
#include <list>

class ItemOwner : public GameObject
{
protected:
    std::list<Item*> items;
    
    virtual bool can_carry(const Item* item) const;

    ItemOwner(); // ItemOwners shouldn't be instantiatable

public:
    bool addItem(Item *i);
    void removeItem(Item* i);
        
    virtual ~ItemOwner();
};

#endif
