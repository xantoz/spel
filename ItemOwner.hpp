#ifndef ITEMOWNER_HPP
#define ITEMOWNER_HPP

#include "Item.hpp"
#include <list>

class ItemOwner 
{
protected:
    std::list<Item*> items;
    
    virtual bool can_carry(const Item* item) const;

    ItemOwner(); // ItemOwners shouldn't be instantiatable

public:
    bool add_item(Item *i);
    void removeItem(Item* i);
        
    virtual ~ItemOwner();
};

#endif
