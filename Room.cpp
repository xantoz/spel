#include "Room.hpp"
#include <algorithm>



Room *Room::getExit(std::string direction) const
{
    // TODO: or use exceptions instead to signal non-existant exits

    auto it = exits.find(direction);
    if (it == exits.end())
        return nullptr; // TODO: instead throw exception here?
    return it->second;
}

void Room::addActor(Actor *a)
{
    actors.push_front(a);
    // we're friends with the actors guild
    if (a->room == nullptr)
        a->room->removeActor(a);
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
}

const std::list<Actor*> &Room::getActors()
{
    return actors;
}

