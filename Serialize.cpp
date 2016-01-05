#include "Serialize.hpp"

#include "Actor.hpp"

#include <map>
#include <vector>
#include <tuple>
#include <list>
#include <sstream>
#include <iostream>
#include <unordered_map>

std::string gensym()
{
    static unsigned n = 0;
    std::stringstream ss;
    ss << "G";
    ss << n++;
    return ss.str();
}

std::string escapeString(const std::string &string)
{
    std::ostringstream ss;
    for (auto c: string)
    {
        switch (c)
        {
            case '"':
                ss << "\\\"";
                break;
            case '\n':
                ss << "\\n";
                break;
            case '\\':
                ss << "\\\\";
                break;
            default:
                ss << c;
                break;
        }
    }
    return ss.str();
}

static std::string unescapeString(const std::string &string)
{
    if (string == "") return "";

    std::ostringstream ss;
    auto it = string.begin();
    for (; it != (string.end() - 1); ++it)
    {
        switch (*it)
        {
            case '\\':
                switch (*(it+1))
                {
                    case '"':
                        ss << '"';
                        break;
                    case 'n':
                        ss << '\n';
                        break;
                    default:
                        ss << *(it+1);
                        break;
                }
                if (++it == (string.end() - 1)) goto break_loop;
                break;
            default:
                ss << *it;
                break;
        }
    }
  break_loop:
    
    switch (*it)
    {
        case '\\':
            std::cerr << "Warning: Encountered improperly escaped string." << std::endl;
            break;
        default:
            ss << *it;
            break;
    }
        
    return ss.str();
}

std::string stringify(const std::string &string)
{
    std::ostringstream ss;
    ss << '"' << escapeString(string) << '"';
    return ss.str();
}

static std::string serializeActor(const Actor *actor, std::ostream &os)
{
    std::string sym = gensym();
    os << sym;
    actor->serialize(os);
    return sym;
}

void serialize(const std::list<Room*> &rooms, std::ostream &os)
{
    std::unordered_map<const Room*, std::string> room_to_sym;
    std::vector<std::tuple<std::string, const Actor*> > actors;
    
    for (const Room *room: rooms)
    {
        std::string roomSym = gensym();
        room_to_sym[room] = roomSym;
        os << roomSym << ":MAKE-ROOM " << stringify(room->getName()) << " " << stringify(room->getBaseDescription()) << std::endl;
        for (const Actor *actor: room->getActors())
        {
            std::string actorSym = actor->serialize(os);
            os << ":ADD-ACTOR " << roomSym << " " << actorSym << std::endl;
            actors.emplace_back(actorSym, actor);           // save all actors together with their syms for later use
        }
        // ItemOwner::serializeItems(std::ostream&, const std::string&) const
        room->serializeItems(os, roomSym);
    }
    // Link the room graph
    for (const Room *room: rooms)
    {
        for (auto const &ent: room->getExits())
        {
            os << ":SET-EXIT"          << " "
               << room_to_sym.at(room) << " "
               << stringify(ent.first) << " "
               << room_to_sym.at(ent.second) << std::endl;
        }
    }
    // Set actor death-exits
    for (auto &tup: actors)
    {
        for (auto &ent: std::get<1>(tup)->getDeathExits())
        {
            os << ":SET-DEATH-EXIT " << std::get<1>(tup) << " " << stringify(ent.first) << " " << room_to_sym.at(ent.second) << std::endl;
        }
    }
             
    
}
