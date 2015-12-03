#include "Actor.hpp"
#include "Room.hpp"
#include "exceptions.hpp"
#include <iostream>

Actor::Actor(const std::string &name, const std::string &description) :
    ItemOwner(name, description)
{
}

Actor(const std::string &name, const std::string &description, const Stats &_stats) :
    ItemOwner(name, description), stats(_stats)
{
}


Actor::~Actor()
{
    std::cerr << "Actor<" << getName() << "> destructor" << std::endl;
    
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

const Stats &getStats()
{
    return stats;
}

// void setStats(const Stats &stats)
// {
//     this->stats = stats;
// }

