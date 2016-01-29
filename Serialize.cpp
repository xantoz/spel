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
#include "Callback.hpp"

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
#include <limits>
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
                    args.emplace_back(std::move(unescapeString(std::string(start, it))));
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

// note that this is a very stupid format and there are lots of undefined scenarios that may
// make the whole program crash, leak memory or worse. Like not creating a Player instance in
// the world file, creating an Item and not assigning it to an ItemOwner, or creating an Actor
// without assigning it to a Room and probably more.
void load(std::istream &is, std::initializer_list<std::pair<const std::string, GameObject*>> predef_vars)
{
    unsigned row = 1; // program counter that also doubles as indicating row for printouts. NOTE: indexed from 1!!

    std::unordered_map<std::string, unsigned> labels;
    std::unordered_map<std::string, GameObject*> vars(predef_vars);
    auto get_label = [&] (const std::string label) {
        auto it = labels.find(label);
        if (it == labels.end()) throw InvalidFileException(row, "Jumping to non-existant label.");
        return it->second - 1; // compensate for row being iterated directly after an executed command
    };
    std::unordered_map<std::string, std::function<GameObject*(const std::vector<std::string> &)> > cmds = {
        // Ignore all args and return NULL. Use for comments.
        {"REM", [&](const std::vector<std::string> &args) { return nullptr; }},
        // Unconditional jump
        {"GOTO", [&](const std::vector<std::string> &args) {
                row = get_label(args.at(0)); // change current row.
                return nullptr;
            }
        },
        // Conditional. In the two arg form it jumps to the label only if the first argument is
        // non-null (true). In the three arg form it jumps to the first label (argument 2) if first
        // argument is non-null (true) else it jumps to the the second label (third argument) 
        {"IF", [&](const std::vector<std::string> &args) {  
                if (args.size() == 2 || args.size() == 3)
                {
                    if (vars.at(args[0]) != nullptr)
                        row = get_label(args[1]);
                    else if (args.size() == 3)
                        row = get_label(args[2]);
                }
                else
                    throw InvalidFileException(row, "Wrong amount of args.");
                return nullptr;
            }
        },
        // inline if, only has a three arg form. returns the value of the second arg if first arg is true
        // else it returns the value of the third arg
        {"?", [&](const std::vector<std::string> &args) {   
                return (vars.at(args.at(0)) != nullptr) ? vars.at(args.at(1)) : vars.at(args.at(2));
            }
        },
        // Stop processing the script here, as if the file ended after this command
        {"STOP",
         [&](const std::vector<std::string> &args) {
                // Setting the row value to maximum should do the trick. Remember to subtract one
                // since we'll increment row after this, otherwise we'll run into a nasty bug.
                row = std::numeric_limits<unsigned>::max() - 1; 
                return nullptr;
            }
        },
        // :DESTROY <GAMEOBJECT>. Destroy something
        {"DESTROY", [&](const std::vector<std::string> &args) {
                delete vars.at(args.at(0));
                return nullptr;
            }
        },
        // :TELEPORT <ACTOR> <ROOM>. Teleports actor in first arg to the room in the second arg
        {"TELEPORT", [&](const std::vector<std::string> &args) {
                Actor *a = dynamic_cast<Actor*>(vars.at(args.at(0)));
                Room *r = dynamic_cast<Room*>(vars.at(args.at(1)));
                if (a == nullptr || r == nullptr)
                    throw new InvalidFileException(row, "Expected an Actor in the first argument, and a Room in the second.");
                r->addActor(a);
                return nullptr;
            }
        },
        // :GO <ACTOR> "<DIRECTION>". Makes actor go in the direction direction.
        {"GO",  [&](const std::vector<std::string> &args) {
                Actor *a = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (a == nullptr)
                    throw new InvalidFileException(row, "Expected an Actor in the first argument.");
                a->go(args.at(1));
                return nullptr;
            }
        },
        // <VAR>:GET-ROOM-BY-NAME "<ROOM NAME>"
        {"GET-ROOM-BY-NAME", [&](const std::vector<std::string> &args) {
                auto it = std::find_if(Room::getRooms().begin(), Room::getRooms().end(),
                                       [&](Room *r) { return r->getName() == args.at(0); });
                return (it == Room::getRooms().end()) ? nullptr : *it;
            }
        },
        // <VAR>:GET-ROOM-FROM-ACTOR <ACTOR>
        {"GET-ROOM-FROM-ACTOR", [&](const std::vector<std::string> &args) {
                Actor *a = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (a == nullptr)
                    throw new InvalidFileException(row, "Expected an Actor in the first argument.");
                return a->getRoom();
            }
        },
        // <VAR>:GET-ACTOR <ROOM VARIABLE> "<ACTOR NAME>"
        {"GET-ACTOR", [&](const std::vector<std::string> &args) {
                Room *room = dynamic_cast<Room*>(vars.at(args.at(0)));
                if (room == nullptr) throw InvalidFileException(row, "Expected a Room in the first argument.");
                return room->getActor(args.at(1));
            }
        },
        // <VAR>:GET-ACTOR-GLOBAL "<ACTOR NAME>". Will get an actor by name, no matter what room  it's in.
        {"GET-ACTOR-GLOBAL", [&](const std::vector<std::string> &args) {
                auto actors = Actor::getActors();
                auto it = std::find_if(actors.begin(), actors.end(), [&](Actor *a) {
                        return a->getName() == args.at(0);
                    });
                return (it != actors.end()) ? *it : nullptr;
            }
        },
        // <VAR>:GET-PLAYER. Simply gets the player from the global player pointer.
        {"GET-PLAYER", [&](const std::vector<std::string> &args) {
                return player;
            }
        },
        // <VAR>:GET-ITEM <ITEMOWNER VARIABLE> "<ITEM NAME>"
        {"GET-ITEM", [&](const std::vector<std::string> &args) {
                ItemOwner *io = dynamic_cast<ItemOwner*>(vars.at(args.at(0)));
                if (io == nullptr) throw InvalidFileException(row, "Expected an ItemOwner in the first argument.");
                return io->getItem(args.at(1));
            }
        },
        // We might need to summon a null now that we have conditionals and stuff
        {"GET-NULL", [&](const std::vector<std::string> &args) {
                if (args.size() != 0) throw InvalidFileException(row, "Too many args.");
                return nullptr;
            }
        },
        // <VAR>:NAME-EQ <GameObjectRef> "String"
        // Returns nullptr if name doesn't equal. Returns an undefined non-null ptr if name is
        // equal (don't dereference this, strictly for use with IF)
        {"NAME-EQ", [&](const std::vector<std::string> &args) {
                return (vars.at(args.at(0))->getName() == args.at(1)) ? (GameObject*)(-1) : nullptr;
            }
        },
        // :SET-STATS <ActorRef> <Stats>
        {"SET-STATS", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(row, "Expect first argument to be Actor.");
                actor->setStats(parseStats(args.at(1)));
                return nullptr;
            }
        },
        // :SET-HP <ActorRef> <HP (int)>
        {"SET-STATS", [&](const std::vector<std::string> &args) {
                Actor *actor = dynamic_cast<Actor*>(vars.at(args.at(0)));
                if (actor == nullptr) throw InvalidFileException(row, "Expect first argument to be Actor.");
                actor->setHP(std::stoi(args.at(1)));
                return nullptr;
            }
        },
        // :SET-DESCRIPTION <GameObjectRef> <Description (string)>
        {"SET-DESCRIPTION", [&](const std::vector<std::string> &args) {
                vars.at(args.at(0))->setDescription(args.at(1));
                return nullptr;
            }
        },
        // :APPEND-DESCRIPTION <ActorRef> <Description (string)>
        {"APPEND-DESCRIPTION", [&](const std::vector<std::string> &args) {
                vars.at(args.at(0))->appendDescription(args.at(1));
                return nullptr;
            }
        },
        {"MESSAGE", [&](const std::vector<std::string> &args) {
                std::cout << args.at(0) << std::endl;
                return nullptr;
            }
        },
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
        {"MAKE-SHOP", [&](const std::vector<std::string> &args) {
                std::list<std::pair<Item*, unsigned>> items;
                for(std::size_t i = 2; i < args.size(); i+=2)
                {
                    Item* item = dynamic_cast<Item*>(vars.at(args[i]));
                    items.emplace_back(item, std::stoi(args[i+1]));
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
                if (player != nullptr) delete player;       // this makes it possible to MAKE-PLAYER multiple times without leaking memory (but why would you?)
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
        {"MAKE-CALLBACK-HUMAN", [&](const std::vector<std::string> &args) {
                if      (args.size() == 3) return new CallbackHuman(args.at(0), args.at(1), args.at(2));
                else if (args.size() == 4) return new CallbackHuman(args.at(0), args.at(1), parseStats(args.at(2)), args.at(3));
                else if (args.size() == 5) return new CallbackHuman(args.at(0), args.at(1), parseStats(args.at(2)), std::stoi(args.at(3)), args.at(4));
                else                       throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        {"MAKE-ITEM", [&](const std::vector<std::string> &args) {
                return new Item(args.at(0), args.at(1), std::stoi(args.at(2)));
            }
        },
        {"MAKE-CALLBACK-ITEM", [&](const std::vector<std::string> &args) {
                if (args.size() == 3)
                    return new CallbackItem(args.at(0), args.at(1), std::stoi(args.at(2)));
                else if (args.size() == 4)
                    return new CallbackItem(args.at(0), args.at(1), std::stoi(args.at(2)), args.at(3));
                else
                    throw InvalidFileException(row, "Wrong amount of args.");
            }
        },
        // :SET-CONSUMABLE <CALLBACKITEM> [01]
        {"SET-CONSUMABLE", [&](const std::vector<std::string> &args) {
                CallbackItem *ci = dynamic_cast<CallbackItem*>(vars.at(args.at(0)));
                if (ci == nullptr)
                    throw InvalidFileException(row, "Expected first arg to be a CallbackItem");
                ci->setConsumable(std::stoi(args.at(1)));
                return nullptr;
            }
        },
        // :SET-USED <CALLBACKITEM> [01]
        {"SET-USED", [&](const std::vector<std::string> &args) {
                CallbackItem *ci = dynamic_cast<CallbackItem*>(vars.at(args.at(0)));
                if (ci == nullptr)
                    throw InvalidFileException(row, "Expected first arg to be a CallbackItem");
                ci->setUsed(std::stoi(args.at(1)));
                return nullptr;
            }
        },
        {"SET-CALLBACK", [&](const std::vector<std::string> &args) {
                Callback *cb = dynamic_cast<Callback*>(vars.at(args.at(0)));
                if (cb == nullptr)
                    throw InvalidFileException(row, "Expected first arg to be a Callback");
                cb->setCallback(args.at(1));
                return nullptr;
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
                switch (args.size())
                {
                    case 1: return new Potion(std::stoi(args.at(0)));
                    case 2: return new Potion(args.at(0), std::stoi(args.at(1)));
                    default: throw InvalidFileException(row, "Wrong amount of args.");
                }
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
        
    struct linestruct
    {
        std::string var_name, cmd_name;
        std::vector<std::string> args;
        linestruct(std::string &&a, std::string &&b, std::vector<std::string> &&c) :
            var_name(a), cmd_name(b), args(c)
        {
        }
    };
    std::vector<struct linestruct> lines = {};
    std::string line;
    while (!is.eof()) 
    {
        // new variables each pass so we can move construct from them.
        // default values for the empty row case
        std::string var_name = "";
        std::string cmd_name = "";
        std::vector<std::string> args = {}; 
                
        getline(is, line);
        if (line.find_first_not_of(' ') != std::string::npos) 
        { // non-empty row 
            auto colon = line.begin();
            for (; colon != line.end() && *colon != ':'; ++colon);
            if (colon == line.end())
                throw InvalidFileException(row, "Row without colon.");
            auto dot = line.begin(); // if there's a dot somewhere in the colon part it's a label for jumping
            for (; dot != colon && *dot != '.'; ++dot);
            if (dot != colon)
            { // we found a dot, we have a label on our hands
                labels.emplace(std::string(line.begin(), dot), row);
                var_name = std::string(dot+1, colon);
            }
            else
            {
                var_name = std::string(line.begin(), colon);
            }
            parseCmd(std::string(colon+1, line.end()), cmd_name, args);
        }
        
        lines.emplace_back(std::move(var_name), std::move(cmd_name), std::move(args));
        ++row;
    }

    row = 1; // reset row counter/PC. remember that row is indexed from 1 (so that it is usable for printouts)
    for (; row <= lines.size(); ++row)
    {
        auto &line = lines[row-1];
        
        // std::cerr << row << " VAR_NAME: " << stringify(line.var_name) << " CMD: " << stringify(line.cmd_name) << " ARGS: ";
        // for (std::string &arg: line.args)
        //     std::cerr << "\"" << arg << "\" ";
        // std::cerr << std::endl;

        if (line.cmd_name == "")
            continue;
        try 
        {
            GameObject *result = cmds.at(line.cmd_name)(line.args);
            if (line.var_name.size() > 0)
                vars[line.var_name] = result;
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
}

void serialize(const std::list<Room*> &rooms, std::ostream &os)
{
    std::unordered_map<const Room*, std::string> room_to_sym;
    std::vector<std::tuple<std::string, const Actor*> > actors;
    std::vector<std::tuple<std::string, const ItemOwner*> > itemOwners;

    // Create all rooms and the actors in there. Put the actors in the appropriate rooms.
    for (const Room *room: rooms)
    {
        std::string roomSym = room->serialize(os);
        room_to_sym[room] = roomSym;
        for (const Actor *actor: room->getActors())
        {
            std::string actorSym = actor->serialize(os);
            os << ":ADD-ACTOR " << roomSym << " " << actorSym << std::endl;
            actors.emplace_back(actorSym, actor);           // save all actors together with their syms for later use
            itemOwners.emplace_back(actorSym, actor);
        }
        itemOwners.emplace_back(roomSym, room);
    }
    // Create all the Items owned by ItemOwners, and add them to their respective owners. This needs
    // to happen after Rooms have been created since keys can reference rooms.
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
