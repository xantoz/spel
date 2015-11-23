#include "ItemOwner.hpp"
#include <iostream>
#include <algorithm>

bool ItemOwner::can_carry(const Item* item) const
{
    return true;
}

ItemOwner::ItemOwner()
{
}

ItemOwner::ItemOwner(const std::string &name, const std::string &description) : GameObject(name, description)
{
}

bool ItemOwner::addItem(Item *i)
{
    // TODO: or use exception?
    if (!this->can_carry(i))
        return false;
                
    items.push_front(i);
    if (i->owner != nullptr) // items can be ownerless (though normally only directly after being constructed
        i->owner->removeItem(i);
    i->owner = this;

    return true;
}

void ItemOwner::removeItem(Item *i)
{
    // TODO: instead of returning bool throw exception when it doesn't exist
        
    auto it = std::find(items.begin(), items.end(), i);
    if (it == items.end())
    {
        // TODO: THROW NoSuchItemException EXCEPTION
    }
    items.erase(it);
}

Item *ItemOwner::getItem(const std::string &name)
{
    return std::find(items.begin(), items.end(),
                     [&](Item *) {
                         return item->getName() == name;
                     });
}


ItemOwner::~ItemOwner()
{
    std::cerr << "ItemOwner destructor" << std::endl;
        
    for (Item *i: items)
        delete i;
}

