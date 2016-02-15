#ifndef ROOM_HPP
#define ROOM_HPP

#include "ItemOwner.hpp"
#include "Actor.hpp"
#include <string>
#include <list>
#include <map>

struct EncounterProbability
{
    double troll;
    double thief;
    double golem;
    double dragon;
    EncounterProbability();
    EncounterProbability(double troll, double thief, double golem, double dragon);
    std::string serializeString() const;
};
  
class Room : public ItemOwner 
{
private:
    std::map<std::string, Room*> exits;
    std::list<Actor*> actors;

    static std::list<Room*> rooms;

    EncounterProbability encounters;
    
public:
    // Room(const std::string &name, const std::string &description);
    Room(const std::string &name, const std::string &description, ...);
    Room(const std::string &name, const std::string &description, const EncounterProbability &encounters);
    Room(const std::string &name, const std::string &description, int lvl);
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

    void update();

    static const std::list<Room*> &getRooms();

    const EncounterProbability &getEncounterProbs() const;

    virtual std::string serialize(std::ostream &os) const;

    // friend void main_loop();
};

#endif 
