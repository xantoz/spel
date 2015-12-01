#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "ItemOwner.hpp"
#include <string>

class Room;                                                 // solve circular reference
class Actor : public ItemOwner 
{
private:
    Room *room;
protected:
    unsigned strength;
public:
    Actor(const std::string &name, const std::string &description);
    virtual ~Actor() override;
    
    void go(std::string direction);
    Room *getRoom() const;
    std::string use(const std::string &itemName);

    unsigned getStrength() const;
    void setStrength(unsigned str);


    friend class Room;
};

#endif
