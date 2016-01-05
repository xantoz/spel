#include "Room.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <stdarg.h>
#include <iostream>

// Room::Room(const std::string &name, const std::string &description) :
//     ItemOwner(name, description)
// {
// }

Room::Room(const std::string &name, const std::string &description, ...) :
    ItemOwner(name, description)
{
    va_list list;

    va_start(list, description);

    char *exit_str;
    Room *exit_room;
    exit_str  = va_arg(list, char*);
    exit_room = va_arg(list, Room*);
    while(exit_str != NULL)
    {
        exits[std::string(exit_str)] = exit_room;
        exit_str  = va_arg(list, char*);
        exit_room = va_arg(list, Room*);
    }

    // Add ourselves to the list of rooms
    rooms.push_front(this);
}

Room::~Room()
{
    std::cerr << "Room<" << getName() << "> destructor" << std::endl;\

    for (auto const &exit: exits)
        exit.second->removeExit(this);
    
    // Since destructing actors will automatically remove them from the list of actors (see Actor::~Actor)
    while (actors.size() != 0)
        delete actors.front();

    // Remove ourselves from the list of rooms
    rooms.remove(this);
}

const std::map<std::string, Room*> &Room::getExits() const
{
    return exits;
}

Room *Room::getExit(const std::string &direction) const
{
    auto it = exits.find(direction);
    if (it == exits.end())
        return nullptr; 
    return it->second;
}

void Room::setExit(const std::string &direction, Room *room)
{
    exits[direction] = room;
}

void Room::removeExit(const std::string &direction)
{
    if (exits.find(direction) == exits.end())
        throw NoSuchExitException(); // TODO: possibly more severe than regular NoSuchExitException
    exits.erase(direction);
}

void Room::removeExit(const Room *room)
{
    std::map<std::string, Room*>::iterator
        it = std::find_if(exits.begin(), exits.end(),
                          [&](const std::pair<std::string, Room*> &item) -> bool { return item.second == room; });
    if (it == exits.end())
        throw NoSuchExitException(); // TODO: possibly more severe than regular NoSuchExitException
    exits.erase(it);
}

void Room::addActor(Actor *a)
{
    actors.push_front(a);
    // we're friends with the actors guild
    if (a->room != nullptr)
        a->room->removeActor(a);
    a->room = this;
}

void Room::removeActor(Actor *a)
{
    auto it = std::find(actors.begin(), actors.end(), a);
    if (it == actors.end())
    {
        throw NoSuchActorException();
    }
    actors.erase(it);
}

Actor *Room::getActor(const std::string &actor)
{
    auto it = std::find_if(actors.begin(), actors.end(),
                           [&](const Actor *a) { return a->getName() == actor; });
    return (it == actors.end()) ? nullptr : *it;
}


const std::list<Actor*> &Room::getActors() const
{
    return actors;
}


std::string Room::getDescription() const
{
    std::string desc = GameObject::getDescription();
    if(getItems().size() > 0 || getActors().size() > 0)
    {
        desc += "\nYou see: ";
    }
    
    if (getItems().size() > 0)
    {
        auto it = getItems().begin();
        desc += (*it)->getName();
        ++it;
        for (; it != getItems().end(); ++it)
            desc += ", " + (*it)->getName();
        desc += " on the floor.";
    }
    if(getActors().size() > 0)
    {
        if(getItems().size() > 0)
            desc += " and ";
        auto it = getActors().begin();
        desc += (*it)->getName();
        ++it;
        for (; it != getActors().end(); ++it)
            desc += ", " + (*it)->getName();
    }
    desc += "\n\nObvious Exits are:";
    for (auto &exit: exits)
        desc += " " + exit.first;

    return desc;
}

std::list<Room*> Room::rooms;

const std::list<Room*> &Room::getRooms()
{
    return Room::rooms;
}
