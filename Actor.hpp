#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "ItemOwner.hpp"
#includ "Stats.hpp"
#include <string>

class Room;                                                 // solve circular reference
class Actor : public ItemOwner 
{
private:
    Room *room;

protected:    
    Stats stats;
    
public:
    Actor(const std::string &name, const std::string &description);
    Actor(const std::string &name, const std::string &description, const Stats &stats);
    virtual ~Actor() override;
    
    void go(std::string direction);
    Room *getRoom() const;
    std::string use(const std::string &itemName);
    std::string pickup(const std::string &itemName);
    std::string drop(const std::string &itemName);
    
    const Stats &getStats() const;
    // void setStats(const Stats &stats);

    friend class Room;
};

#endif
