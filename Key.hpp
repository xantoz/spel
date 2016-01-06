#ifndef KEY_HPP
#define KEY_HPP

#include "Item.hpp"
#include <ostream>
#include <string>
#include <unordered_map>

class Room;
class Key : public Item 
{
private:
    Room *fromRoom;
    std::string fromRoomDirection;
    Room *toRoom;
    std::string toRoomDirection;
    
public:
    Key(const std::string &name,
        const std::string &description,
        unsigned weight);
    Key(const std::string &name,
        const std::string &description,
        unsigned weight,
        Room *fromRoom, const std::string &fromRoomDirection);
    Key(const std::string &name,
        const std::string &description,
        unsigned weight,
        Room *fromRoom, const std::string &fromRoomDirection,
        Room *toRoom, const std::string &toRoomDirection);
    virtual ~Key() override;

    void setAction(Room *fromRoom, const std::string &fromRoomDirection,
                   Room* toRoom, const std::string &toRoomDirection);
    Room *getFromRoom() const;
    Room *getToRoom() const;
    void setFromRoom(Room *room);
    void setToRoom(Room *room);
    const std::string &getFromRoomDirection() const;
    const std::string &getToRoomDirection() const;
    
    virtual std::string use(Actor *actor) override;
    virtual std::string serialize(std::ostream &os) const override;
    // This is a special case where we need the room_to_sym stuff to be able to utilize the full-length MAKE-KEY constructor.
    std::string serialize(std::ostream &os, const std::unordered_map<const Room*, std::string> &room_to_sym) const;
};
    
#endif
