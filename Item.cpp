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

//////////////
//// ITEM ////
//////////////
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

void Item::use(Actor* actor)
{
    static std::string asdf = "This item is useless.";
    std::cout << asdf << std::endl;
    return;
}

std::string Item::getDescription() const
{
    return GameObject::getDescription() + "\nWeight: " + std::to_string(weight);
}

std::string Item::serialize(std::ostream &os) const
{
    std::string itemSym = gensym();
    os << itemSym << ":MAKE-ITEM "
       << stringify(getName()) << " "
       << stringify(getBaseDescription()) << " "
       << getWeight() << std::endl;
    return itemSym;
}


//////////////////////
//// CALLBACKITEM ////
//////////////////////
CallbackItem::CallbackItem(const std::string &name,
             const std::string &description,
             unsigned weight) :
    CallbackItem(name, description, weight, "")
{
}

CallbackItem::CallbackItem(const std::string &name,
             const std::string &description,
             unsigned weight,
             const std::string &callback_path) :
    Item(name, description, weight), Callback(callback_path)
{
}

CallbackItem::~CallbackItem()
{
    std::cerr << "CallbackItem<" << getName() << "> destructor" << std::endl;
}

void CallbackItem::use(Actor* actor)
{
    // Pass the actor using the item/having the item used on self as ACTOR
    // Pass the item being used as THIS
    runCallback({{"ACTOR", actor}, {"THIS", this}});
}
    
std::string CallbackItem::serialize(std::ostream &os) const 
{
    std::string itemSym = gensym();
    os << itemSym << ":MAKE-CALLBACK-ITEM "
       << stringify(getName()) << " "
       << stringify(getBaseDescription()) << " "
       << getWeight() << " "
       << stringify(getCallback()) << std::endl;
    os << ":SET-CONSUMABLE " << itemSym << " " << consumable << std::endl;
    os << ":SET-USED " << itemSym << " " << used << std::endl;
    
    return itemSym;
}

void CallbackItem::setConsumable(bool bol)
{
    consumable = bol;
}
void CallbackItem::setUsed(bool bol)
{
    used = bol;
}

