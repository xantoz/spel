#include "Item.hpp"

#include "exceptions.hpp"

#include "Equippable.hpp"
#include "Sword.hpp"
#include "Shoes.hpp"
#include "Armor.hpp"
#include "Shield.hpp"
#include "ItemOwner.hpp"
#include "Actor.hpp"

#include <iostream>
#include <typeinfo>

Item::Item(std::string name, std::string description, unsigned w) :
    GameObject(name, description),
    weight(w),
    owner(nullptr)
{
}

// NOTE: Don't delete items before unequipping them, things will go bad...
Item::~Item()
{
    std::cerr << "Item<" << getName() << "> destructor" << std::endl;
    
    if (owner != nullptr)
    {
        owner->removeItem(this);
    }
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


std::string Item::serialize(std::ostream &os) const
{
    os << "<<<TODO>>>";
    return "TODO";
}
