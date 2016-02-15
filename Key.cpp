#include "Key.hpp"

#include "Serialize.hpp"
#include "Room.hpp"
#include <iostream>

Key::Key(const std::string &name,
         const std::string &description,
         unsigned weight) :
    Key(name, description, weight, nullptr, "", nullptr, "")
{
}

Key::Key(const std::string &name,
    const std::string &description,
    unsigned weight,
    Room *fromRoom, const std::string &fromRoomDirection) :
    Key(name, description, weight, fromRoom, fromRoomDirection, nullptr, "")
{
}

Key::Key(const std::string &name,
         const std::string &description,
         unsigned weight,
         Room *fr, const std::string &frd,
         Room *tr, const std::string &trd) :
    Item(name, description, weight),
    fromRoom(fr),
    fromRoomDirection(frd),
    toRoom(tr),
    toRoomDirection(trd)
{
    consumable = true;
}

Key::~Key()
{
    std::cerr << "Key<" << getName() << "> destructor" << std::endl;
}

void Key::setAction(Room *fr, const std::string &frdir,
                    Room* tr, const std::string &trdir)
{
    fromRoom = fr;
    fromRoomDirection = frdir;
    toRoom = tr;
    toRoomDirection = trdir;
}

void Key::use(Actor *actor)
{
    if (fromRoom == nullptr)
    {
        std::cout << "This key is unusable." << std::endl;
        return;
    }
    if (actor->getRoom() != fromRoom)
    {
        std::cout << "No door in this room will fit this key" << std::endl;
        return;
    }

    fromRoom->setExit(fromRoomDirection, toRoom);
    if (toRoom != nullptr)
        toRoom->setExit(toRoomDirection, fromRoom);
    
    used = true;

    std::cout << "A door going " << fromRoomDirection << " has openened." << std::endl;
    fromRoom->appendDescription("You opened a door to " + toRoom->getName() + " here.");
    toRoom->appendDescription("You used a key to get here from " + fromRoom->getName() + ".");
    return;
}

// Special serializer that can utilize the full-length MAKE-KEY constructor by being given
// several extra parameters from the serializer.
std::string Key::serialize(std::ostream &os, const std::unordered_map<const Room*, std::string> &room_to_sym) const
{
    std::string itemSym = gensym();
    os << itemSym << ":MAKE-KEY "
       << stringify(getName()) << " "
       << stringify(getBaseDescription()) << " "
       << getWeight();
    if (fromRoom != nullptr)
    {
        os << " " << room_to_sym.at(fromRoom) << " " << stringify(fromRoomDirection);
        if (toRoom != nullptr)
            os << " " << room_to_sym.at(toRoom) << " " << stringify(toRoomDirection);
    }
    os << std::endl;
    return itemSym;
}

std::string Key::serialize(std::ostream &os) const
{
    // Due to how the file format is made we might need to refer to yet
    // non-created rooms and thus the constructor used in the save-file
    // format doesn't take the last four parameters, instead opting to set
    // them at a later time.
    std::string sym = gensym();
    os << sym << ":MAKE-KEY "
       << stringify(getName()) << " "
       << stringify(getBaseDescription()) << " "
       << getWeight() << std::endl;
    return sym;
}


Room *Key::getFromRoom() const
{
    return fromRoom;
}

Room *Key::getToRoom() const
{
    return toRoom;
}

void Key::setFromRoom(Room *room)
{
    fromRoom = room;
}

void Key::setToRoom(Room *room)
{
    toRoom = room;
}

const std::string &Key::getFromRoomDirection() const
{
    return fromRoomDirection;
}

const std::string &Key::getToRoomDirection() const
{
    return toRoomDirection;
}
