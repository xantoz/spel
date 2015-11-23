#include "Actor.hpp"
#include "Room.hpp"
#include "exceptions.hpp"

void Actor::go(std::string direction)
{
    Room *nroom = room->getExit(direction);
    if (nroom == nullptr)
        throw NoSuchExitException();
    room->removeActor(this);
    nroom->addActor(this);                              // this sets room to nroom
}

const std::string &Actor::use(const std::string &itemName)
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
