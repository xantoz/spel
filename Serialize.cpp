#include "Serialize.hpp"

#include "main.hpp"
#include "Actor.hpp"
#include "exceptions.hpp"
#include "GameObject.hpp"
#include "Room.hpp"
#include "Item.hpp"
#include "Equippable.hpp"
#include "Sword.hpp"
#include "Shoes.hpp"
#include "Shield.hpp"
#include "Armor.hpp"
#include "Classes.hpp"
#include "Potion.hpp"
#include "Key.hpp"
#include "Shop.hpp"

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
    while (true)
    {
        if (it == str.end())
        {
            if (start != it && std::none_of(start, it, [](char c) { return c == ' '; }))
                args.emplace_back(start, it);
            break;
        }
        else if (*it == '"')
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
    if (numbers.size() != 7) throw InvalidFileException("Stats-param of wrong length");
    Stats stats = {numbers.at(0), numbers.at(1), numbers.at(2), numbers.at(3), numbers.at(4), numbers.at(5), numbers.at(6)};
    return stats;
}

EncounterProbability parseEncounterProbs(const std::string &str)
{
    std::vector<std::string> strings; strings.reserve(4);
    std::vector<double> numbers;      numbers.reserve(4);
    boost::split(strings, str, boost::is_any_of(";"));
    std::transform(strings.begin(), strings.end(), back_inserter(numbers), [](const std::string &a) -> int { return std::stod(a); });
    if (numbers.size() != 4) throw InvalidFileException("EncounterProbability-param of wrong length");
    return EncounterProbability(numbers.at(0), numbers.at(1), numbers.at(2), numbers.at(3));;
}

void load(std::istream &is)
{
    unsigned row = 0;
    bool made_player = false;
    
    std::unordered_map<std::string, GameObject*> vars;
    std::unordered_map<std::string, std::function<GameObject*(const std::vector<std::string> &)> > cmds = {
        {"MAKE-ROOM", [&](const std::vector<std::string> &args) {
                if (args.size() == 2)
                    return new Room(args.at(0), args.at(1), nullptr);
                else if (args.size() == 3) {
                    if (args.at(2).find_first_of(';') != std::string::npos) // third argument is an EncounterProbability
                        return new Room(args.at(0), args.at(1), parseEncounterProbs(args.at(2)));
                    else                 // third argument is a level
                        return new Room(args.at(0), args.at(1), std::stoi(args.at(2)));
                }
                else
                    throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-SHOP", [&](const std::vector<std::string> &args) 
         {
             std::list<std::pair<Item*, unsigned>> items;
             for(std::size_t i = 2; i < args.size(); i+=2)
             {
                 Item* item = dynamic_cast<Item*>(vars.at(args[i]));
                 items.push_back(std::pair<Item*, unsigned>(item, std::stoi(args[i+1])));
             }
             
             return new Shop(args.at(0), args.at(1), items);
         }
        },
        
        {"MAKE-ACTOR", [&](const std::vector<std::string> &args) {
                if      (args.size() == 2) return new Actor(args.at(0), args.at(1));
                else if (args.size() == 3) return new Actor(args.at(0), args.at(1), parseStats(args.at(2)));
                else if (args.size() == 4) return new Actor(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-PLAYER", [&](const std::vector<std::string> &args) {
                // This sets the player global variable. Remember that it's important to always MAKE-PLAYER in all files.
                made_player = true;
                if (player != nullptr) delete player;       // this makes it possible to MAKE-PLAYER several times (but why would you?)
                if      (args.size() == 3) player = new Player(args.at(0), args.at(1), parseStats(args.at(2)));
                else if (args.size() == 4) player = new Player(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
                return player;
            }
        },
        {"MAKE-TROLL", [&](const std::vector<std::string> &args) {
                if      (args.size() == 2) return new Troll(args.at(0), std::stoi(args.at(1)));
                else if (args.size() == 4) return new Troll(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-DRAGON", [&](const std::vector<std::string> &args) {
                if      (args.size() == 2) return new Dragon(args.at(0), std::stoi(args.at(1)));
                else if (args.size() == 4) return new Dragon(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-THIEF", [&](const std::vector<std::string> &args) {
                if      (args.size() == 2) return new Thief(args.at(0), std::stoi(args.at(1)));
                else if (args.size() == 4) return new Thief(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-GOLEM", [&](const std::vector<std::string> &args) {
                if      (args.size() == 2) return new Golem(args.at(0), std::stoi(args.at(1)));
                else if (args.size() == 4) return new Golem(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-HUMAN", [&](const std::vector<std::string> &args) {
                if      (args.size() == 3) return new Human(args.at(0), args.at(1), args.at(2));
                else if (args.size() == 4) return new Human(args.at(0), args.at(1), parseStats(args.at(2)), args.at(3));
                else if (args.size() == 5) return new Human(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)), args.at(4));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-ITEM", [&](const std::vector<std::string> &args) {
                return new Item(args.at(0), args.at(1), std::stoi(args.at(2)));
            }
        },
        {"MAKE-EQUIPPABLE", [&](const std::vector<std::string> &args) {
                return new Equippable(args.at(0), args.at(1), std::stoi(args.at(2)), parseStats(args.at(3)));
            }
        },
        {"MAKE-SHIELD", [&](const std::vector<std::string> &args) {
                return new Shield(args.at(0), args.at(1), std::stoi(args.at(2)), parseStats(args.at(3)));
            }
        },
        {"MAKE-SWORD", [&](const std::vector<std::string> &args) {
                return new Sword(args.at(0), args.at(1), std::stoi(args.at(2)), parseStats(args.at(3)));
            }
        },
        {"MAKE-SHOES", [&](const std::vector<std::string> &args) {
                return new Shoes(args.at(0), args.at(1), std::stoi(args.at(2)), parseStats(args.at(3)));
            }
        },
        {"MAKE-ARMOR", [&](const std::vector<std::string> &args) {
                return new Armor(args.at(0), args.at(1), std::stoi(args.at(2)), parseStats(args.at(3)));
            }
        },
        {"MAKE-POTION", [&](const std::vector<std::string> &args) {
                return new Potion(args.at(0), std::stoi(args.at(1)));
            }
        },
        {"MAKE-KEY", [&](const std::vector<std::string> &args) {
                if (args.size() == 3)
                    return new Key(args.at(0), args.at(1), std::stoi(args.at(2)));
                else if (args.size() == 5)
                {
                    Room *fromRoom = dynamic_cast<Room*>(vars.at(args.at(3)));
                    if (fromRoom == nullptr) throw InvalidFileException(row, "Expected 4th arg to be Room.");
                    return new Key(args.at(0), args.at(1), std::stoi(args.at(2)), fromRoom, args.at(4));
                }
                else if (args.size() == 7)
                {
                    Room *fromRoom = dynamic_cast<Room*>(vars.at(args.at(3)));
                    Room *toRoom = dynamic_cast<Room*>(vars.at(args.at(5)));
                    if (fromRoom == nullptr) throw InvalidFileException(row, "Expected 4th arg to be Room.");
                    if (toRoom == nullptr) throw InvalidFileException(row, "Expected 6th arg to be Room.");
                    return new Key(args.at(0), args.at(1), std::stoi(args.at(2)),
                                   fromRoom, args.at(4),
                                   toRoom, args.at(6));
                }
                else
                    throw InvalidFileException(row, "Wrong amount of arguments.");
            }
        },
        {"SET-NUMBER-KILLS", [&](const std::vector<std::string> &args) 
         {
             Player *player = dynamic_cast<Player*>(vars.at(args.at(0)));
             if(player == nullptr) throw InvalidFileException(row, "Trying to pass non-Player to SET-NUMBER-KILLS.");
             player->setKills(std::stoi(args.at(1)));
             return nullptr;
         }
        },
        {"SET-KILLED-MONSTER", [&](const std::vector<std::string> &args)
         {
             Player *player = dynamic_cast<Player*>(vars.at(args.at(0)));
             if(player == nullptr) throw InvalidFileException(row, "Trying to pass non-Player to SET-KILLED-MONSTER.");
             player->setKilledMonster(std::stoi(args.at(1)));
             return nullptr;
         }
        },
        
        {"SET-DROP", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(row, "Trying to pass non-Actor to SET-DROP.");
                actor->setDrop(std::stoi(args.at(1)));
                return nullptr;
            }
        },
        {"SET-EXIT", [&](const std::vector<std::string> &args) {
                Room *fromRoom = dynamic_cast<Room*>(vars.at(args.at(0)));
                Room *toRoom   = dynamic_cast<Room*>(vars.at(args.at(2)));
                if (fromRoom == nullptr) throw InvalidFileException(row, "Expected Room as first arg to SET-EXIT.");
                if (toRoom == nullptr)   throw InvalidFileException(row, "Expected Room as third arg to SET-EXIT.");
                fromRoom->setExit(args.at(1), toRoom);
                return nullptr;
            }
        },
        {"SET-DEATH-EXIT", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                Room *room = dynamic_cast<Room*>(vars.at(args.at(2)));
                if (actor == nullptr) throw InvalidFileException(row, "Expected Actor as first arg to SET-DEATH-EXIT.");
                if (room == nullptr) throw InvalidFileException(row, "Expected Room as third arg to SET-DEATH-EXIT.");
                actor->setDeathExit(args.at(1), room);
                return nullptr;
            }
        },
        {"ADD-ACTOR", [&](const std::vector<std::string> &args) {
                Room *room = dynamic_cast<Room*>(vars.at(args.at(0)));
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(1)));
                if (room == nullptr) throw InvalidFileException(row, "Expected Room as first arg to ADD-ACTOR.");
                if (actor == nullptr) throw InvalidFileException(row, "Expected Actor as second arg to ADD-ACTOR.");
                room->addActor(actor);
                return nullptr;
            }
        },
        {"ADD-ITEM", [&](const std::vector<std::string> &args) {
                ItemOwner *itemOwner = dynamic_cast<ItemOwner*>(vars.at(args.at(0)));
                Item *item = dynamic_cast<Item*>(vars.at(args.at(1)));
                if (itemOwner == nullptr) throw InvalidFileException(row, "Expected ItemOwner as first arg to ADD-ITEM.");
                if (item == nullptr) throw InvalidFileException(row, "Expected Item as second arg to ADD-ITEM.");
                itemOwner->addItem(item);
                return nullptr;
            }
        },
        {"EQUIP-SWORD", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(row, "Expected actor as first arg.");
                actor->equipSword(args.at(1));
                return nullptr;
            }
        },
        {"EQUIP-SHIELD", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(row, "Expected actor as first arg.");
                actor->equipShield(args.at(1));
                return nullptr;
            }
        },
        {"EQUIP-SHOES", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(row, "Expected actor as first arg.");
                actor->equipShoes(args.at(1));
                return nullptr;
            }
        },
        {"EQUIP-ARMOR", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(row, "Expected actor as first arg.");
                actor->equipArmor(args.at(1));
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
            throw InvalidFileException(row, "Row without colon.");
        var_name = line.substr(0, pos);
        parseCmd(line.substr(pos + 1), cmd_name, args);

        // std::cerr << "VAR_NAME: " << stringify(var_name) << " CMD: " << stringify(cmd_name) << " ARGS: ";
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
            throw InvalidFileException(row, "No such command/var. Or too few parameters. what(): " + std::string(e.what()));
        }
        catch (const std::invalid_argument &e)
        {
            throw InvalidFileException(row, "invalid_argument: " + std::string(e.what()));
        }
    }

    if (made_player == false)
        throw InvalidFileException("No MAKE-PLAYER call in the file.");
}


// void serialize(const std::list<Room*> &rooms, std::ostream &os)
// {
//     std::unordered_map<const Room*, std::string> room_to_sym;
//     std::vector<std::tuple<std::string, const Actor*> > actors;
//     // std::vector<std::tuple<std::string, const Key*> > keys;
//    
//     for (const Room *room: rooms)
//     {
//         std::string roomSym = gensym();
//         room_to_sym[room] = roomSym;
//         os << roomSym << ":MAKE-ROOM " << stringify(room->getName()) << " " << stringify(room->getBaseDescription()) << std::endl;
//         for (const Actor *actor: room->getActors())
//         {
//             std::string actorSym = actor->serialize(os);
//             os << ":ADD-ACTOR " << roomSym << " " << actorSym << std::endl;
//             actors.emplace_back(actorSym, actor);           // save all actors together with their syms for later use
//         }
//         // ItemOwner::serializeItems(std::ostream&, const std::string&) const
//         room->serializeItems(os, roomSym);
//     }
//     // Link the room graph
//     for (const Room *room: rooms)
//     {
//         for (auto const &ent: room->getExits())
//         {
//             os << ":SET-EXIT"          << " "
//                << room_to_sym.at(room) << " "
//                << stringify(ent.first) << " "
//                << room_to_sym.at(ent.second) << std::endl;
//         }
//     }
//     // Set actor death-exits
//     for (auto &tup: actors)
//     {
//         for (auto &ent: std::get<1>(tup)->getDeathExits())
//         {
//             os << ":SET-DEATH-EXIT " << std::get<0>(tup) << " " << stringify(ent.first) << " " << room_to_sym.at(ent.second) << std::endl;
//         }
//     }
// }

void serialize(const std::list<Room*> &rooms, std::ostream &os)
{
    std::unordered_map<const Room*, std::string> room_to_sym;
    std::vector<std::tuple<std::string, const Actor*> > actors;
    std::vector<std::tuple<std::string, const ItemOwner*> > itemOwners;

    // Create all rooms and the actors in there. Put the actors in the appropriate rooms.
    for (const Room *room: rooms)
    {
        std::string roomSym = gensym();
        room_to_sym[room] = roomSym;
        os << roomSym << ":MAKE-ROOM "
           << stringify(room->getName()) << " "
           << stringify(room->getBaseDescription()) << " " 
           << room->getEncounterProbs().serializeString() << std::endl;
        for (const Actor *actor: room->getActors())
        {
            std::string actorSym = actor->serialize(os);
            os << ":ADD-ACTOR " << roomSym << " " << actorSym << std::endl;
            actors.emplace_back(actorSym, actor);           // save all actors together with their syms for later use
            itemOwners.emplace_back(actorSym, actor);
        }
        itemOwners.emplace_back(roomSym, room);
    }
    // Create all the Items owned by ItemOwners, and add them to their respective owners
    for (auto &itemOwnerTuple: itemOwners)
    {
        const ItemOwner *itemOwner = std::get<1>(itemOwnerTuple);
        const std::string &itemOwnerSym = std::get<0>(itemOwnerTuple);
        for (Item *item: itemOwner->getItems())
        {
            std::string itemSym;
            Key* key;
            if ((key = dynamic_cast<Key*>(item)))
            {
                // we can create keys in one row here if we can call the (non-virtual)
                // Key::serialize(std::ostream&, const std::unordered_map<const Room*, std::string>&)
                itemSym = key->serialize(os, room_to_sym);
            }
            else
            {
                itemSym = item->serialize(os);
            }
            os << ":ADD-ITEM " << itemOwnerSym << " " << itemSym << std::endl;
        }
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
