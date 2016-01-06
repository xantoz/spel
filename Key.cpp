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

std::string Key::use(Actor *actor)
{
    return "";
}

std::string Key::serialize(std::ostream &os)
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
