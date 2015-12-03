#include "Item.hpp"
#include "ItemOwner.hpp"
#include <iostream>

Item::Item(std::string name, std::string description, unsigned w) :
    GameObject(name, description),
    weight(w),
    owner(nullptr)
{
}


Item::Item(std::string name, std::string description, unsigned w, ItemOwner *o) :
    GameObject(name, description),
    weight(w)
{
    // this will set this->owner = o
    o->addItem(this);                                       
}

Item::~Item()
{
    std::cerr << "Item<" << getName() << "> destructor" << std::endl;
    
    if (owner != nullptr)
        owner->removeItem(this);
}

unsigned Item::getWeight() const
{
    return weight;
}

ItemOwner *Item::getOwner() const
{
    return owner;
}

std::string Item::use()
{
    static std::string asdf = "This item is useless.";
    return asdf;
}


