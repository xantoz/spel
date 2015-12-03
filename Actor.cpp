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

    // Destruct any equipped items
    if (armor != nullptr) delete armor;
    if (shield != nullptr) delete shield;
    if (sword != nullptr) delete sword;
    if (shoes != nullptr) delete shoes;
}

bool Actor::equipArmor(const std::string &name)
{
    Armor *armor = dynamic_cast<Armor*>(this->getItem(name));
    if (nullptr == armor) return false; // Either doesn't exist or isn't an armor
    this->removeItem(armor);                 // remove from inventory (Note: this leaves the owner pointer intact)
    this->armor = armor;
    return true;
}

bool Actor::equipShield(const std::string &name)
{
    Shield *shield = dynamic_cast<Shield*>(this->getItem(name));
    if (nullptr == shield) return false; // Either doesn't exist or isn't a shield
    this->removeItem(shield);                 // remove from inventory (Note: this leaves the owner pointer intact)
    this->shield = shield;
    return true;
}

bool Actor::equipSword(const std::string &name)
{
    Sword *sword = dynamic_cast<Sword*>(this->getItem(name));
    if (nullptr == sword) return false; // Either doesn't exist or isn't a sword
    this->removeItem(sword);                 // remove from inventory (Note: this leaves the owner pointer intact)
    this->sword = sword;
    return true;
}

bool Actor::equipShoes(const std::string &name)
{
    Shoes *shoes = dynamic_cast<Shoes*>(this->getItem(name));
    if (nullptr == shoes) return false; // Either doesn't exist or isn't a pair of shoes
    this->removeItem(shoes); // remove from inventory (Note: this leaves the owner pointer intact)
    this->shoes = shoes;
    return true;
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
    std::cerr << "itemsme" << i->getDescription() << std::endl;
    bool res = this->addItem(i);
    if (res == false)
        std::cout << "Can't carry any more" << std::endl;
    return res;
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
    ret += getBaseDescription() + "\n\n";
    ret += "STATS: " + stats.toString() + "\n";
    ret += "INVENTORY:";
    for (Item *i: getItems())
        ret += " " + i->getName();
    ret += "\n";
    return ret;
}

