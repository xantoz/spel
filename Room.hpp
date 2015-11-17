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
    std::string name;
    std::string description;
    
    std::map<std::string, Room*> exits;
    std::list<Actor*> actors;
public:
    Room *getExit(std::string direction) const
    {
        // TODO: or use exceptions instead to signal non-existant exits

        if (exits.find(direction) == exits.end())
            return nullptr;
        return exits.at(direction);
    }

    void addActor(Actor *a)
    {
        actors.push_front(a);
        // we're friends with the actors guild
        if (a->room == nullptr)
            a->room->remove_actor(a);
        a->room = this;
    }

    void removeActor(Actor *a)
    {
        auto it = std::find(actors.begin(), actors.end(), a);
        if (it == actors.end())
        {
            // TODO: THROW EXCEPTION
        }
        actors.erase(it);
        return true;
    }

    const std::list<Actor*> &getActors()
    {
        return actors;
    }

    // friend void main_loop();
};

#endif 
