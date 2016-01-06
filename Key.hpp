#ifndef KEY_HPP
#define KEY_HPP

#include "Item.hpp"
#include <ostream>
#include <string>

class Room;
class Key : public Item 
{
private:
    Room *fromRoom;
    std::string fromRoomDirection;
    Room *toRoom;
    std::string toRoomDirection;

    // KLUDGE: hack to keep track of key syms
    
    
public:
    Key(const std::string &name,
        const std::string &description,
        unsigned weight);
    Key(const std::string &name,
        const std::string &description,
        unsigned weight,
        Room *fromRoom, const std::string &fromRoomDirection,
        Room *toRoom, const std::string &toRoomDirectionx);
    virtual ~Key() override;

    void setAction(Room *fromRoom, const std::string &fromRoomDirection,
                   Room* toRoom, const std::string &toRoomDirection);
    virtual std::string use(Actor *actor) override;
    virtual std::string serialize(std::ostream &os) const override;
};
    
#endif
