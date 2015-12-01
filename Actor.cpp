#include "Actor.hpp"
#include "Room.hpp"
#include "exceptions.hpp"

Actor::Actor(const std::string &name, const std::string &description) :
    ItemOwner(name, description)
{
}

Actor::~Actor()
{
    if (room != nullptr)
        room->removeActor(this);
}

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


Room *Actor::getRoom() const
{
    return room;
}

unsigned Actor::getStrength() const
{
    return strength;
}

void Actor::setStrength(unsigned str)
{
    strength = str;
}
