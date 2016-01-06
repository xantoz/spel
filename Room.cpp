#include "Room.hpp"

#include "Key.hpp"
#include "Classes.hpp"
#include "exceptions.hpp"

#include <algorithm>
#include <stdarg.h>
#include <iostream>
#include <sstream>
#include <random>
#include <chrono>

EncounterProbability::EncounterProbability() :
    dragon(0.0), thief(0.0), golem(0.0), troll(0.0)
{
}


EncounterProbability::EncounterProbability(double dr, double th, double gol, double tr) :
    dragon(dr), thief(th), golem(gol), troll(tr)
{
}

std::string EncounterProbability::serializeString() const
{
    std::ostringstream ss;
    ss << dragon << ";" << thief << ";" << golem << ";" << troll;
    return ss.str();
}

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

Room::Room(const std::string &name, const std::string &description, const EncounterProbability &enc) :
    ItemOwner(name, description), encounters(enc)
{
    // Add ourselves to the list of rooms
    rooms.push_front(this);
}

Room::Room(const std::string &name, const std::string &description, int lvl) :
    ItemOwner(name, description)
{
    // TODO: make me better, somehow (Kai?)
    encounters.dragon = lvl*0.01;
    encounters.thief = lvl*0.2;
    encounters.golem = lvl*0.05;
    encounters.troll = lvl*0.1;

    // Add ourselves to the list of rooms
    rooms.push_front(this);
}

Room::~Room()
{
    std::cerr << "Room<" << getName() << "> destructor" << std::endl;


    // used to garbage collect ourselves from keys referring to us
    auto key_findalator = [&](Item *it) -> void {
        Key *key = dynamic_cast<Key*>(it);
        if (key != nullptr) {
            if (this == key->getFromRoom())
                key->setFromRoom(nullptr);
            if (this == key->getToRoom())
                key->setToRoom(nullptr);
        }
    };
            
    // Garbage collect ourselves from all other rooms' exits, and the Actors' deathExits
    for (Room *room: Room::getRooms())
    {
        try {
            std::for_each(room->getItems().begin(), room->getItems().end(), key_findalator);
            for (Actor *actor: room->getActors())
            {
                std::for_each(actor->getItems().begin(), actor->getItems().end(), key_findalator);
                try { actor->removeDeathExit(this); }
                catch (const NoSuchExitException &e) { /* ignore */ }
            }
            room->removeExit(this);
        }
        catch (const NoSuchExitException &e) {
            // ignore NoSuchExitException since were asking all rooms and all actors, even those
            // who don't have exits to us, to remove us from their list
        }
    }
    
    auto it = actors.begin();
    while (it != actors.end())
        delete *it++; // deleting removes the actor from the list

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

const EncounterProbability &Room::getEncounterProbs() const
{
    return encounters;
}

void Room::update()
{
    static unsigned cntr = 0;

    // std::cerr << "Updating room " << getName() << std::endl;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::uniform_real_distribution<double> distr(0.0, 1.0);
    std::uniform_int_distribution<int> lvl(1,3);

    if (distr(engine) < encounters.dragon)
        this->addActor(new Dragon("dragon" + std::to_string(++cntr), lvl(engine)));
    
    if (distr(engine) < encounters.thief)
        this->addActor(new Thief("thief" + std::to_string(++cntr), lvl(engine)));
    
    if (distr(engine) < encounters.golem)
        this->addActor(new Golem("golem" + std::to_string(++cntr), lvl(engine)));
    
    if (distr(engine) < encounters.troll)
        this->addActor(new Troll("troll" + std::to_string(++cntr), lvl(engine)));
}


