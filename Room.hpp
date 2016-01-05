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

    static std::list<Room*> rooms;
    
public:
    // Room(const std::string &name, const std::string &description);
    Room(const std::string &name, const std::string &description, ...);
    virtual ~Room() override;

    /// Returns nullptr for non-existant exits
    Room *getExit(const std::string &direction) const;
    const std::map<std::string, Room*> &getExits() const;
    void setExit(const std::string &direction, Room *room);
    void removeExit(const std::string &direction);
    void removeExit(const Room *room);

    /// Throws NoSuchActorException for non-existant actors
    void removeActor(Actor *a);
    void addActor(Actor *a);
    Actor *getActor(const std::string &actor);
    const std::list<Actor*> &getActors() const;

    virtual std::string getDescription() const override;

    static const std::list<Room*> &getRooms();

    // friend void main_loop();
};

#endif 
