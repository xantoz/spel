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

// Item::Item(std::string name, std::string description, unsigned w, ItemOwner *o) :
//     GameObject(name, description),
//     weight(w)
// {
//     // this will set this->owner = o
//     o->addItem(this);                                       
// }

Item::~Item()
{
    std::cerr << "Item<" << getName() << "> destructor" << std::endl;
    
    if (owner != nullptr)
    {
        try 
        {
            owner->removeItem(this);
        }
        catch (const NoSuchItemException &e)
        {
            Actor *actor = dynamic_cast<Actor*>(this->owner);
            if (actor == nullptr) throw e; // just re-throw in the event we're not owned by an actor, something wierd is happening for sure

            std::cerr << "WOOP " << typeid(this->owner).name() << " " << typeid(this).name() << std::endl;
            // If we end up here we better be an equippable and equipped, otherwise something wierd is happening
            if (nullptr == dynamic_cast<Equippable*>(this))
            {
                throw GameException(this->getName() + " Item::~Item: This should never happen. Being destructed but we're neither an Equippabble nor in our owner's bag");
            }
            else if (Sword *sword = dynamic_cast<Sword*>(this))
            {
                if (actor->getSword() != sword) throw GameException("Item::~Item: Sword should be equipped but isn't.");
                actor->sword = nullptr;
            }
            else if (Armor *armor = dynamic_cast<Armor*>(this))
            {
                if (actor->getArmor() != armor) throw GameException("Item::~Item: Armor should be equipped but isn't.");
                actor->armor = nullptr;
            }
            else if (Shield *shield = dynamic_cast<Shield*>(this))
            {
                if (actor->getShield() != shield) throw GameException("Item::~Item: Shield should be equipped but isn't.");
                actor->shield = nullptr;
            }
            else if (Shoes *shoes = dynamic_cast<Shoes*>(this))
            {
                if (actor->getShoes() != shoes) throw GameException("Item::~Item: Shoes should be equipped but aren't.");
                actor->shoes = nullptr;
            }
            else
            {
                throw GameException("Item::~Item: This should happen even less... I don't know how the fuck we ended up here.");
            }
        }
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


