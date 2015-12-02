#include "Item.hpp"
#include "ItemOwner.hpp"
#include <iostream>

Item::Item(std::string name, std::string description, unsigned w) :
    GameObject(name, description),
    owner(nullptr),
    weight(w)
{
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

std::string Item::use()
{
    static std::string asdf = "This item is useless.";
    return asdf;
}


