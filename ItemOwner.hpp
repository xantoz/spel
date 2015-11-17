#ifndef ITEMOWNER_HPP
#define ITEMOWNER_HPP

#include "Item.hpp"
#include <list>

class ItemOwner 
{
protected:
    virtual bool can_carry(const Item* item) const
    {
        return true;
    }
    
    std::list<Item*> items;

public:
    bool add_item(Item *i)
    {
        // TODO: or use exception
        if (!this->can_carry(i))
            return false;
                
        items.push_front(i);
        if (i->owner != nullptr) // items can be ownerless (though normally only directly after being constructed
            i->owner->removeItem(i);
        i->owner = this;

        return true;
    }
    
    void removeItem(Item* i)
    {
        // TODO: instead of returning bool throw exception when it doesn't exist
        
        auto it = std::find(items.begin(), items.end(), i);
        if (it == items.end())
        {
            // TODO: THROW EXCEPTION
        }
        items.erase(it);
    }

    virtual ~ItemOwner()
    {
        std::cerr << "ItemOwner destructor" << std::endl;
        
        for (Item *i: items)
            delete i;
    }
};

#endif
