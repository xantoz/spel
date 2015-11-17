#include "Actor.hpp"
#include "Room.hpp"

void Actor::go(std::string direction)
{
    // TODO: use exceptions instead of returning false?
        
    Room *nroom = room->getExit(direction);
    if (nroom == nullptr)
    {
        // TODO: throw exception
    }
    room->removeActor(this);
    nroom->addActor(this);                              // this sets room to nroom
}

Room *Actor::getRoom() const
{
    return room;
}
