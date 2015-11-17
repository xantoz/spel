#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "Item.hpp"
#include <string>

class Actor : public ItemOwner 
{
private:
    Room *room;
    
public:
    void go(std::string direction)
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

    Room *getRoom()
    {
        return room;
    }

    friend class Room;
};

#endif
