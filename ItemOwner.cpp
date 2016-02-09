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
        

    // // items will remove themselves from the ItemOwner
    // while (items.size() != 0)
    //     delete items.front();

    auto it = items.begin();
    while (it != items.end())
        delete *it++; // deleting removes the item from the list
}

bool ItemOwner::addItem(Item *i)
{
    if (!this->can_carry(i))
        return false;

    addItemNoFail(i);

    return true;
}

void ItemOwner::addItemNoFail(Item *i)
{
    items.push_front(i);
    if (i->owner != nullptr) // items can be ownerless (though normally only directly after being constructed
        i->owner->removeItem(i);
    i->owner = this;
}

void ItemOwner::removeItem(Item *i)
{
    auto it = std::find(items.begin(), items.end(), i);
    if (it == items.end())
    {
        throw NoSuchItemException();
    }
    Item *item = *it;
    items.erase(it);
    item->owner = nullptr;                                  // this item is no longer owned by anybody
}

Item *ItemOwner::getItem(const std::string &name) const
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

void ItemOwner::serializeItems(std::ostream &os, const std::string &ownerSym) const
{
    for (Item *item: items)
    {
        std::string itemSym = item->serialize(os);
        os << ":ADD-ITEM " << ownerSym << " " << itemSym << std::endl;
    }
}

