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
    ItemOwner(const std::string name, const std::string description);

public:
    bool addItem(Item *i);
    void removeItem(Item* i);

    Item *getItem(const std::string &name);
        
    virtual ~ItemOwner();
};

#endif
