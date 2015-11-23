#ifndef ROOM_HPP
#define ROOM_HPP

#include "ItemOwner.hpp"
#include "Actor.hpp"
#include <string>
#include <list>
#include <map>

class Room : public ItemOwner 
{
private:
    std::map<std::string, Room*> exits;
    std::list<Actor*> actors;
    
public:
    Room(const std::string &name, const std::string &description, ...);
    
    Room *getExit(const std::string &direction) const;
    void setExit(const std::string &direction, Room *room);
    void addActor(Actor *a);
    void removeActor(Actor *a);
    const std::list<Actor*> &getActors();

    // friend void main_loop();
};

#endif 
