#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "ItemOwner.hpp"
#include <string>

class Room;                                                 // solve circular reference
class Actor : public ItemOwner 
{
private:
    Room *room;

public:
    void go(std::string direction);
    Room *getRoom() const;
    const std::string &use(const std::string &itemName);

    friend class Room;
};

#endif
