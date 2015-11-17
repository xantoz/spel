#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "Item.hpp"
#include <string>

class Actor : public ItemOwner 
{
private:
    Room *room;
    
public:
    void go(std::string direction);

    friend class Room;
};

#endif
