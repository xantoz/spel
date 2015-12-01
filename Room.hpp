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
    virtual ~Room() override;

    /// Returns nullptr for non-existant exits
    Room *getExit(const std::string &direction) const; 
    void setExit(const std::string &direction, Room *room);
    void removeExit(const std::string &direction);
    void removeExit(const Room *room);

    /// Throws NoSuchActorException for non-existant actors
    void removeActor(Actor *a);
    void addActor(Actor *a);

    virtual std::string getDescription() const override;

    const std::list<Actor*> &getActors();

    // friend void main_loop();
};

#endif 
