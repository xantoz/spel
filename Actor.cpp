#include "Actor.hpp"
#include "Room.hpp"
#include "exceptions.hpp"
#include <iostream>

Actor::Actor(const std::string &name, const std::string &description) :
    ItemOwner(name, description)
{
}

Actor::Actor(const std::string &name, const std::string &description, const Stats &_stats) :
    ItemOwner(name, description), stats(_stats)
{
}

Actor::~Actor()
{
    std::cerr << "Actor<" << getName() << "> destructor" << std::endl;
    
    if (room != nullptr)
        room->removeActor(this);
}

bool Actor::equipArmor(const std::string &name)
{
    return false;                                           // TODO
}

bool Actor::equipShield(const std::string &name)
{
    return false;                                           // TODO
}

bool Actor::equipSword(const std::string &name)
{
    return false;                                           // TODO
}

bool Actor::equipShoes(const std::string &name)
{
    return false;                                           // TODO
}


const Armor *Actor::getArmor() const { return armor; }
const Shield *Actor::getShield() const { return shield; }
const Sword *Actor::getSword() const { return sword; }
const Shoes *Actor::getShoes() const { return shoes; }


void Actor::go(std::string direction)
{
    Room *nroom = room->getExit(direction);
    if (nroom == nullptr)
        throw NoSuchExitException();
    nroom->addActor(this); // this moves the actor to nroom (sets this->room)
}

std::string Actor::use(const std::string &itemName)
{
    Item *item = this->getItem(itemName);
    if (item == nullptr)
        throw NoSuchItemException();
    return item->use();
}

bool Actor::pickup(const std::string &itemName)
{
    if (room == nullptr) return false;
    Item *i = room->getItem(itemName);
    if (i == nullptr) return false;
    return this->addItem(i);
}

bool Actor::drop(const std::string &itemName)
{
    if (room == nullptr) return false;
    Item *i = this->getItem(itemName);
    if (i == nullptr) return false;
    return room->addItem(i);
}

Room *Actor::getRoom() const
{
    return room;
}

const Stats &Actor::getStats() const
{
    return stats;
}

std::string Actor::getDescription() const
{
    std::string ret = "";
    ret += "Name: " + getName() + "\n\n";
    ret += getBaseDescription();
    ret += "STATS: <TODO>\n";
    ret += "INVENTORY:";
    for (Item *i: getItems())
        ret += " " + i->getName();
    ret += "\n";
    return ret;
}

