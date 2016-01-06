#include "Item.hpp"

#include "exceptions.hpp"

#include "Equippable.hpp"
#include "Sword.hpp"
#include "Shoes.hpp"
#include "Armor.hpp"
#include "Shield.hpp"
#include "ItemOwner.hpp"
#include "Actor.hpp"
#include "Serialize.hpp"

#include <iostream>
#include <typeinfo>

Item::Item(const std::string &name, const std::string &description, unsigned w) :
    GameObject(name, description),
    weight(w),
    owner(nullptr),
    used(false),
    consumable(false)
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

bool Item::usedUp() const
{
    return consumable && used;
}

std::string Item::use(Actor* actor)
{
    static std::string asdf = "this item is useless";
    return asdf;
}


std::string Item::serialize(std::ostream &os) const
{
    std::string itemSym = gensym();
    os << itemSym << ":MAKE-ITEM " << stringify(getName()) << " " << stringify(getBaseDescription()) << " " << getWeight() << std::endl;
    return itemSym;
}
