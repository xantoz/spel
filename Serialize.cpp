#include "Serialize.hpp"

#include "main.hpp"
#include "Actor.hpp"
#include "exceptions.hpp"
#include "GameObject.hpp"
#include "Room.hpp"

// #include <map>
#include <vector>
#include <tuple>
#include <list>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <exception>
#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


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

static void parseCmd(const std::string &str, std::string &cmd, std::vector<std::string> &args)
{
    args.clear();
    
    auto it = str.begin();
    for (; it != str.end() && *it != ' '; ++it); // find first space
    cmd = std::string(str.begin(), it);

    while (it != str.end() && *it == ' ') ++it; // skip additional space
    auto start = it;
    while (it != str.end())
    {
        if (*it == '"')
        {
            ++it;
            char prevChar = '"';
            start = it;
            for (; it != str.end(); ++it)
            {
                if (*it == '"' && prevChar != '\\')
                {
                    args.emplace_back(unescapeString(std::string(start, it)));
                    ++it;
                    goto wind_whitespace;
                }
                prevChar = *it;
            }
            throw InvalidFileException("Row ended inside string.");
        }
        else if (*it == ' ')
        {
            args.emplace_back(start, it);
            goto wind_whitespace;
        }
        
        ++it;
        continue;
        
      wind_whitespace:
        while (it != str.end() && *it == ' ') ++it;    // skip additional space
        start = it;
    }
    
}

Stats parseStats(const std::string &str)
{
    std::vector<std::string> strings; strings.reserve(7);
    std::vector<int> numbers;         numbers.reserve(7);
    boost::split(strings, str, boost::is_any_of(";"));
    std::transform(strings.begin(), strings.end(), back_inserter(numbers), [](const std::string &a) -> int { return std::stoi(a); });
    if (numbers.size() != 7) throw new InvalidFileException("Stats-param of wrong length");
    Stats stats = {numbers.at(0), numbers.at(1), numbers.at(2), numbers.at(3), numbers.at(4), numbers.at(5), numbers.at(6)};
    return stats;
}

void load(std::istream &is)
{
    unsigned row = 0;
    
    std::unordered_map<std::string, GameObject*> vars;
    std::unordered_map<std::string, std::function<GameObject*(const std::vector<std::string> &)> > cmds = {
        {"MAKE-ROOM", [&](const std::vector<std::string> &args) {
                return new Room(args.at(0), args.at(1), nullptr);
            }
        },
        {"MAKE-ACTOR", [&](const std::vector<std::string> &args) {
                if      (args.size() == 2) return new Actor(args.at(0), args.at(1));
                else if (args.size() == 3) return new Actor(args.at(0), args.at(1), parseStats(args.at(2)));
                else if (args.size() == 4) return new Actor(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)));
                else                       throw new InvalidFileException(std::to_string(row) + ": Wrong amount of args.");
            }
        },
        {"SET-DROP", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(std::to_string(row) + ": Trying to pass non-Actor to SET-DROP.");
                actor->setDrop(std::stoi(args.at(0)));
                return nullptr;
            }
        },
        {"ADD-ACTOR", [&](const std::vector<std::string> &args) {
                Room *room = dynamic_cast<Room*>(vars.at(args.at(0)));
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(1)));
                if (room == nullptr) throw InvalidFileException(std::to_string(row) + ": Expected Room as first arg to ADD-ACTOR.");
                if (actor == nullptr) throw InvalidFileException(std::to_string(row) + ": Expected Actor as second arg to ADD-ACTOR.");
                room->addActor(actor);
                return nullptr;
            }
        },
        {"ADD-ITEM", [&](const std::vector<std::string> &args) {
                ItemOwner *itemOwner = dynamic_cast<ItemOwner*>(vars.at(args.at(0)));
                Item *item = dynamic_cast<Item*>(vars.at(args.at(1)));
                if (itemOwner == nullptr) throw InvalidFileException(std::to_string(row) + ": Expected ItemOwner as first arg to ADD-ITEM.");
                if (item == nullptr) throw InvalidFileException(std::to_string(row) + ": Expected Item as second arg to ADD-ITEM.");
                itemOwner->addItem(item);
                return nullptr;
            }
        },
        {"EQUIP-SWORD", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(std::to_string(row) + ": Expected actor as first arg.");
                actor->equipSword(args.at(1));
                return nullptr;
            }
        }
    };
        
    std::string line;
    std::string var_name = "";
    std::string cmd_name = "";
    std::vector<std::string> args = {};
    
    while (!is.eof())
    {
        ++row;
        
        getline(is, line);
        if(line.find_first_not_of(' ') == std::string::npos) // this row is only spaces or empty
            continue;

        
        size_t pos = line.find_first_of(':');
        if (pos == std::string::npos)
            throw InvalidFileException(std::to_string(row) + ": Row without colon.");
        var_name = line.substr(0, pos);
        parseCmd(line.substr(pos + 1), cmd_name, args);

        // std::cerr << "VAR_NAME: " << var_name << " CMD: " << cmd_name << " ARGS: ";
        // for (std::string &arg: args)
        //     std::cerr << "\"" << arg << "\" ";
        // std::cerr << std::endl;
        
        try 
        {
            GameObject *result = cmds.at(cmd_name)(args);
            if (var_name.size() > 0)
                vars[var_name] = result;
        }
        catch (const std::out_of_range &e)
        {
            throw InvalidFileException(std::to_string(row) + ": No such command/var. Or too few parameters.");
        }
    }
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
            os << ":SET-DEATH-EXIT " << std::get<0>(tup) << " " << stringify(ent.first) << " " << room_to_sym.at(ent.second) << std::endl;
        }
    }
             
    
}
