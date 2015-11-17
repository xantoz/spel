#include "Room.hpp"



Room::Room *getExit(std::string direction) const
{
    // TODO: or use exceptions instead to signal non-existant exits

    if (exits.find(direction) == exits.end())
        return nullptr;
    return exits.at(direction);
}

void Room::addActor(Actor *a)
{
    actors.push_front(a);
    // we're friends with the actors guild
    if (a->room == nullptr)
        a->room->remove_actor(a);
    a->room = this;
}

void Room::removeActor(Actor *a)
{
    auto it = std::find(actors.begin(), actors.end(), a);
    if (it == actors.end())
    {
        // TODO: THROW EXCEPTION
    }
    actors.erase(it);
    return true;
}

const std::list<Actor*> & Room::getActors()
{
    return actors;
}

// friend void main_loop();

#endif 
