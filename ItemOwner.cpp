#include "ItemOwner.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <algorithm>

bool ItemOwner::can_carry(const Item* item) const
{
    return true;
}

ItemOwner::ItemOwner()
{
}

ItemOwner::ItemOwner(const std::string &name, const std::string &description) :
    GameObject(name, description)
{
}

ItemOwner::~ItemOwner()
{
    std::cerr << "ItemOwner<" << getName() << "> destructor" << std::endl;
        

    // items will remove themselves from the ItemOwner
    while (items.size() != 0)
        delete items.front();
}


bool ItemOwner::addItem(Item *i)
{
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
    auto it = std::find(items.begin(), items.end(), i);
    if (it == items.end())
    {
        throw NoSuchItemException();
    }
    items.erase(it);
}

Item *ItemOwner::getItem(const std::string &name)
{
    auto it = std::find_if(items.begin(), items.end(),
                           [&](const Item *item) {
                               return item->getName() == name;
                           });
    return (it == items.end()) ? nullptr : *it;
}

const std::list<Item*> &ItemOwner::getItems() const
{
    return items;
}


