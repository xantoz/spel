#include "exceptions.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Sword.hpp"
#include "Classes.hpp"
#include "Serialize.hpp"
#include "Potion.hpp"
#include "Key.hpp"
#include "Shop.hpp"

#include <cctype>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <functional>
#include <memory>
#include <vector>

#include <boost/algorithm/string/trim_all.hpp>
#include <boost/algorithm/string/regex.hpp>

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

using namespace std;

enum Mode 
{
    NORMAL, BATTLE, SHOP
} mode;

Player *player;
static Actor *opponent;
static Shop *shop;
static bool winflag = false;

static std::map<const string, function<void(string)>> cmds;
static std::map<const string, function<void(string)>> battleCmds;
static std::map<const string, function<void(string)>> shopCmds(cmds);

#include "Classes.hpp"



static void destroy_everything()
{
    while (Room::getRooms().size() > 0)
    {
        Room *room = Room::getRooms().front();
        cerr << "==Deleting room " << room->getName() << "==" << endl;
        delete room;
    }

    player = nullptr;
    opponent = nullptr;
    shop = nullptr;
    mode = NORMAL;
}

void win()
{
    winflag = true;
}

static void enterBattleMode(Actor *actor)
{
    mode = BATTLE;
    opponent = actor;
    cout << "Entered battle with " << opponent->getName() << endl;
    cout << "Opponent has Stats: " << opponent->getStats().toString() << endl;
}

static void exitBattleMode()
{   
    mode = NORMAL;
    opponent = nullptr;
    cout << "Exited battle" << endl;
}

static void go(string arg)
{
     if (arg == "")
     {
         cout << "Where did you want to go, you said?" << endl;
         return;
     }
     player->go(arg);
     cout << player->getRoom()->getName() << endl;
     cout << player->getRoom()->getDescription() << endl;
     shop = dynamic_cast<Shop*>(player->getRoom());

     if(shop != nullptr)
     {
         mode = SHOP;
     }
     else 
     {
         mode = NORMAL;
     }
}

static void die(string arg)
{
    if (arg == "")
    {
        std::cout << "In a desperate bout of rage you kill yourself\nYou are dead\nRest In Pepperonis." << std::endl;
        player->die();
    }
    else
    {
        std::cout << "???" << std::endl;
    }
}

static void look(string arg)
{
    if (arg == "")
        cout << player->look() << endl;
    else
        cout << player->look(arg) << endl;    
}

static void use_impl(string arg, function<void(void)> callback)
{
    if (arg == "")
    {
        cout << "What do you want to use?" << endl;
        return;
    }
    else 
    {
        vector<string> words;
        boost::algorithm::split_regex(words, arg, boost::regex("\\s+on\\s+"));
        if (words.size() == 1)
            player->use(words.at(0));
        else
            player->use(words.at(0), words.at(1));

        if (callback)
            callback();

        // always non-null since we'd have triggered an exception with player->use for a non-existant item
        Item *item = player->getItem(words.at(0));
        if (item->usedUp())
        {
            std::cout << "You used up the " << item->getName() << "." << std::endl;
            delete item;
        }
    }
}

static void use(string arg)
{
    use_impl(arg, nullptr);
}

static void useBattle(string arg)
{
    use_impl(arg, [](){ opponent->attackResponse(player); });
}

static void pickup(string arg)
{
     if (arg == "")
     {
         cout << "What did you want to pick up?" << endl;
         return;
     }
     if (player->pickup(arg))
         cout << "Picked up " << arg << endl;
     else
         cout << "Cannot pick up " << arg << "." << endl;
         // cout << "I don't see any such thing." << endl;
}

static void drop(string arg)
{
    if (arg == "")
    {
        cout << "What did you want to drop?" << endl;
        return;
    }
    if (player->drop(arg))
        cout << "Dropped " << arg << endl;
    else
        cout << "I'm not carrying anything like that." << endl;
}

static void equip(string arg)
{
    Item *item = player->getItem(arg);
    if (item == nullptr)
    {
        cout << "No such item." << endl;
        return;
    }

    if (dynamic_cast<Sword*>(item) != nullptr)
        player->equipSword(arg);
    else if (dynamic_cast<Shield*>(item) != nullptr)
        player->equipShield(arg);
    else if (dynamic_cast<Shoes*>(item) != nullptr)
        player->equipShoes(arg);
    else if (dynamic_cast<Armor*>(item) != nullptr)
        player->equipArmor(arg);
    else
        cout << "Not equippable." << endl;
}

/*
void equip(string arg)
{
    boost::trim_all(arg);
    size_t first_space = arg.find_first_of(' ');
    if (first_space == string::npos)
    {
        cout << "What did you want to equip?" << endl;
        return;
    }
    string type = arg.substr(0, first_space);
    string what = arg.substr(first_space + 1);

    // cerr << "meep meep" << endl;
    // if (player->getItem(what))
    //     cerr << "BLOOOORP " << typeid(*player->getItem(what)).name() << endl;
    
    if (type == "sword")
    {
        if (!player->equipSword(what))
        {
            cout << "I don't have any such sword." << endl;
            return;
        }
    }
    else if ("shield" == type)
    {
        if (!player->equipShield(what))
        {
            cout << "I don't have any such shield." << endl;
            return;
        }
    }
    else if ("shoes" == type)
    {
        if (!player->equipShoes(what))
        {
            cout << "I don't have any such shoes." << endl;
            return;
        }
    }
    else if ("armor" == type)
    {
        if (!player->equipArmor(what))
        {
            cout << "I don't have any such armor." << endl;
            return;
        }
    }
    else
    {
        cout << "There's no " << type << " kind of equipment in this world." << endl;
        return;
    }

    cout << "Equipped " << what << "." << endl;
}
*/

static void unequip(string arg)
{
    if (arg == "sword")
    {
        player->unequipSword();
        cout << "Unequipped sword." << endl;
    }
    else if (arg == "shield")
    {
        player->unequipShield();
        cout << "Unequipped shield." << endl;
    }
    else if (arg == "shoes")
    {
        player->unequipShoes();
        cout << "Unequipped shoes." << endl; 
    }
    else if (arg == "armor")
    {
        player->unequipArmor();
        cout << "Unequipped armor." << endl; 
    }
    else if (arg == "")
    {
        cout << "What did you want to unequip?" << endl;
    }
    else
    {
        cout << "I can't unequip " << arg << endl;
    }
}

static void battle(string arg)
{
    if(arg == "")
        cout << "Battle whom?" << endl;
    else 
    {
        Actor *actor = player->getRoom()->getActor(arg);
        if(actor != nullptr)
        {
            enterBattleMode(actor);
        }
    }
}

static void attack(string arg)
{

    player->attack(opponent);
}

static void run(string arg)
{
    if (opponent->getTotalStats().spd > player->getTotalStats().spd)
    {
        double rval = std::rand()/(double)RAND_MAX;
        cout << rval << endl;
        
        if (rval < 0.8)
        {
            exitBattleMode();
        }
        else 
        {
            cout << "Could not run away" << endl;
            opponent->attack(player);
        }
    }
    else 
    {
        exitBattleMode();
    }
}

static void talk(string arg)
{
    if(arg == "")
        cout << "Talk with whom?" << endl;
    else 
    {
        Actor *actor = player->getRoom()->getActor(arg);
        Human *human = dynamic_cast<Human*>(actor);
        if(human != nullptr)
        {
            human->talk();
        }
        else 
        {
            cout << "Not a human" << endl;
        }
        
    }
}

/*
template <typename I>
I random_element(I begin, I end)
{
    const unsigned long n = std::distance(begin, end);
    const unsigned long divisor = (RAND_MAX + 1) / n;

    unsigned long k;
    do { k = std::rand() / divisor; } while (k >= n);

    return std::advance(begin, k);
}

void randomSpawn()
{
    static unsigned cntr = 0;
    if (rand() % 5 == 1)
    {
        Actor *act = new Thief("Thief" + std::to_string(++cntr), rand() % 3);
        auto rooms = Room::getRooms();
        // (*random_element(rooms.begin(), rooms.end()))->addActor(act);
                
        unsigned cnt = rand() % rooms.size();
        auto it = rooms.begin();
        for (unsigned i = 0; i < cnt; ++i)
            ++it;
        (*it)->addActor(act);
    }
}
*/

static void save(string filename)
{
    if (filename == "")
        filename = "quicksave.save";
    std::ofstream outfile(filename);
    serialize(Room::getRooms(), outfile);
    cout << "Saved world to " << filename << endl;
}

static void load_world(string filename)
{
    if (filename == "")
        filename = "quicksave.save";
    cout << "Ragnarök devours the known world." << std::endl;
    destroy_everything();
    cout << "Loading from " << filename << endl;
    std::ifstream infile(filename);
    load(infile);
}

// Shop functions
static void listItems(string arg)
{
    cout << shop->listInventory();
}

static void buy(string arg)
{
    if(arg == "")
    {
        cout << "Buy what?" << endl;
    }
    else 
    {
        try
        {
            pair<Item*, unsigned> item = shop->getShopItem(arg);
            if(player->getMoney() >= item.second)
            {
                if (player->addItem(item.first))
                {
                    shop->removeShopItem(item);
                    player->addMoney(-item.second);
                    cout << "You bought the " << item.first->getName()  << "!" << endl;
                }
                else
                {
                    cout << "Sorry, but it looks you can't carry that right now." << endl;
                }
            }
            else 
            {
                cout << "Not enough money!" << endl;
            }
            
        }
        catch (const NoSuchItemException &e)
        {
            cout << "No such item!" << endl;
        }
        
    }
}

static void sell(string arg)
{
    if(arg == "")
    {
        cout << "Sell what?" << endl;
    }
    else 
    {
        Item* item = player->getItem(arg);
        if(item == nullptr)
        {
            cout << "You don't have that item!" << endl;
        }
        else 
        {
            player->addMoney(item->getWeight());
            cout << "You sold the item for " << item->getWeight() << endl;
            player->removeItem(item);
        }    
    }       
}

// cheat cmd for debugging
static void teleport(string arg)
{
    auto it = std::find_if(Room::getRooms().begin(), Room::getRooms().end(),
                           [&](Room *r) { return r->getName() == arg; });
    if (it != Room::getRooms().end())
    {
        std::cout << "Teleporting to " << (*it)->getName() << std::endl;
        (*it)->addActor(player);
    }
}

static void imrich(string arg)
{
    try
    {
        player->addMoney(std::stoi(arg));
    }
    catch (invalid_argument &e)
    {
        cerr << e.what() << endl;
    }
    catch (out_of_range &e)
    {
        cerr << e.what() << endl;
    }
}

static void kill(string arg)
{
    opponent->die();
}


#ifdef USE_READLINE
static void *xmalloc(int size)
{
    void *buf;
 
    buf = malloc(size);
    if (!buf)
    {
        fprintf(stderr, "Error: Out of memory. Exiting.'n");
        exit(1);
    }
 
    return buf;
}

static char *dupstr(const char* s)
{
    char *r;
 
    r = (char*)xmalloc((strlen (s) + 1));
    strcpy(r, s);
    return r;
}

template<class Whatever>
static char const **cmd_from_map(const map<const string, Whatever> &map)
{
    char const **cmd = new const char*[(map.size() + 1)];
    char const **p = cmd;
    for (auto &item: map)
        *p++ = item.first.c_str();
    *p = nullptr;

    return cmd;
}

static char const **cmd_from_vector(const vector<const char*> &v)
{
    char const **cmd = new const char*[(v.size() + 1)];
    char const **p = cmd;
    for (const char *str: v)
        *p++ = str;
    *p = nullptr;
    return cmd;
}

static char const **cmd_look()
{
    vector<const char*> stuff;
    stuff.push_back("self");
    stuff.push_back("myself");
    for (Actor *a: player->getRoom()->getActors())
        stuff.push_back(a->getName().c_str());
    for (Item *i: player->getRoom()->getItems())
        stuff.push_back(i->getName().c_str());
    for (Item *i: player->getItems())
        stuff.push_back(i->getName().c_str());
    return cmd_from_vector(stuff);
}

static char const **cmd_room_items()
{
    vector<const char*> stuff;
    for (Item *i: player->getRoom()->getItems())
        stuff.push_back(i->getName().c_str());
    return cmd_from_vector(stuff);
}

static char const **cmd_player_items()
{
    vector<const char*> stuff;
    for (Item *i: player->getItems())
        stuff.push_back(i->getName().c_str());
    return cmd_from_vector(stuff);
}

static char const **cmd_player_equippables()
{
    vector<const char*> stuff;
    for (Item *i: player->getItems())
        if (dynamic_cast<Equippable*>(i) != nullptr)
            stuff.push_back(i->getName().c_str());
    return cmd_from_vector(stuff);
}

static char const **cmd_room_actors()
{
    vector<const char*> stuff;
    for (Actor *a: player->getRoom()->getActors())
        stuff.push_back(a->getName().c_str());
    return cmd_from_vector(stuff);
}

static char const **cmd_room_humans()
{
    vector<const char*> stuff;
    for (Actor *a: player->getRoom()->getActors())
        if (dynamic_cast<Human*>(a) != nullptr)
            stuff.push_back(a->getName().c_str());
    return cmd_from_vector(stuff);
}

static char const **cmd_room_exits()
{
    vector<const char*> stuff;
    for (auto &item: player->getRoom()->getExits())
        stuff.push_back(item.first.c_str());
    return cmd_from_vector(stuff);
}

static char const **cmd_rooms()
{
    vector<const char*> stuff;
    for (Room *room: Room::getRooms())
        stuff.push_back(room->getName().c_str());
    return cmd_from_vector(stuff);
}

enum generator_mode
{
    CMD, LOOK, USE1, USE2, USE3, PICKUP, EQUIP, UNEQUIP, DROP, BATTLECMD, TELEPORT, GO, TALK
};

static inline char *generator_helper(const char* text, int state, enum generator_mode genmode)
{
    // char* cmd [] = {"hello", "world", "hell" ,"word", "quit", NULL};
    const char** tmp = nullptr;
    switch (genmode)
    {
        default:
        case CMD:
            tmp = (cmd_from_map((mode == BATTLE) ? battleCmds :
                                (mode == SHOP)   ? shopCmds : cmds));
            break;
        case LOOK:
            tmp = cmd_look();
            break;
        case PICKUP:
            tmp = cmd_room_items();
            break;
        case EQUIP:
            tmp = cmd_player_equippables();
            break;
        case UNEQUIP:
            tmp = new const char*[5]{"sword", "shield", "shoes", "armor", NULL};
            break;
        case GO:
            tmp = cmd_room_exits();
            break;
        case DROP:
        case USE1:
            tmp = cmd_player_items();
            break;
        case USE2:
            tmp = new const char*[2]{"on", NULL};
            break;
        case BATTLECMD:
        case USE3:
            tmp = cmd_room_actors();
            break;
        case TELEPORT:
            tmp = cmd_rooms();
            break;
        case TALK:
            tmp = cmd_room_humans();
            break;
    }
    if (tmp == nullptr) return nullptr;
    unique_ptr<const char*[]> cmd(tmp);
    
    static int list_index, len;
    const char *name;
 
    if (!state)
    {
        list_index = 0;
        len = strlen(text);
    }
 
    while (nullptr != (name = cmd[list_index]))
    {
        list_index++;
 
        // if (strncmp(name, text, len) == 0)
        //     return (dupstr(name));
        if (strncasecmp(name, text, len) == 0)
            return (dupstr(name));
    }
 
    /* If no names matched, then return NULL. */
    return ((char *)NULL);
}

static char *generate_cmd(const char* text, int state) { return generator_helper(text, state, CMD); }
static char *generate_look(const char* text, int state) { return generator_helper(text, state, LOOK); }
static char *generate_use1(const char* text, int state) { return generator_helper(text, state, USE1); }
static char *generate_use2(const char* text, int state) { return generator_helper(text, state, USE2); }
static char *generate_use3(const char* text, int state) { return generator_helper(text, state, USE3); }
static char *generate_pickup(const char* text, int state) { return generator_helper(text, state, PICKUP); }
static char *generate_equip(const char* text, int state) { return generator_helper(text, state, EQUIP); }
static char *generate_unequip(const char* text, int state) { return generator_helper(text, state, UNEQUIP); }
static char *generate_drop(const char* text, int state) { return generator_helper(text, state, DROP); }
static char *generate_battle(const char* text, int state) { return generator_helper(text, state, BATTLECMD); }
static char *generate_teleport(const char* text, int state) { return generator_helper(text, state, TELEPORT); }
static char *generate_go(const char* text, int state) { return generator_helper(text, state, GO); }
static char *generate_talk(const char* text, int state) { return generator_helper(text, state, TALK); }

static bool first_word_eq(const char *a, const char *b)
{
    while ((*a != '\0' && !isspace(*a)) && (*b != '\0' && !isspace(*b)))
    {
        if (*a != *b)
            return false;
        
        ++a; ++b;
    }
    return true;
}

// static bool first_word_eq(const char *a, const char *b)
// {
//     while ((*a != '\0') && (*b != '\0' && !isspace(*b)))
//     {
//         if (*a != *b)
//             return false;

//         if (isspace(*a) && (*b == '\0' || isspace(*b)))
//             return true;
            
//         ++a; ++b;
//     }
//     return false;
// }

static unsigned wordcount(const char *a)
{
    unsigned count = 0;
    bool inword = false;
    
    while (*a != '\0')
    {
        if (isspace(*a))
        {
            if (inword)
            {
                inword = false;
                ++count;
            }
        }
        else
        {
            inword = true;
        }
        ++a;
    }

    return count;
}

static char **my_completion(const char *text , int start,  int end)
{
    char **matches;
 
    matches = (char **)NULL;

    // cout << "DERP:" << start << endl;
    // cout << "HERP:" << text << endl;
    // cout << "MERP:" << rl_line_buffer << endl;

    unsigned wc = wordcount(rl_line_buffer);
 
    if (start == 0)
        matches = rl_completion_matches((char*)text, &generate_cmd);
    else if (wc == 1 && first_word_eq(rl_line_buffer, "unequip"))
        matches = rl_completion_matches((char*)text, &generate_unequip);
    else if (wc == 1 && first_word_eq(rl_line_buffer, "look"))
        matches = rl_completion_matches((char*)text, &generate_look);
    else if (wc == 1 && (first_word_eq(rl_line_buffer, "pickup") || first_word_eq(rl_line_buffer, "get")))
        matches = rl_completion_matches((char*)text, &generate_pickup);
    else if (first_word_eq(rl_line_buffer, "use"))
    {
        switch (wc)
        {
            case 1:
                matches = rl_completion_matches((char*)text, &generate_use1);
                break;
            case 2:
                matches = rl_completion_matches((char*)text, &generate_use2);
                break;
            case 3:
                matches = rl_completion_matches((char*)text, &generate_use3);
                break;
        }
    }
    else if (wc == 1 && first_word_eq(rl_line_buffer, "equip"))
        matches = rl_completion_matches((char*)text, &generate_equip);
    else if (wc == 1 && first_word_eq(rl_line_buffer, "drop"))
        matches = rl_completion_matches((char*)text, &generate_drop);
    else if (wc == 1 && first_word_eq(rl_line_buffer, "battle"))
        matches = rl_completion_matches((char*)text, &generate_battle);
    else if (wc == 1 && first_word_eq(rl_line_buffer, "teleport"))
        matches = rl_completion_matches((char*)text, &generate_teleport);
    else if (wc == 1 && first_word_eq(rl_line_buffer, "go"))
        matches = rl_completion_matches((char*)text, &generate_go);
    else if (wc == 1 && first_word_eq(rl_line_buffer, "talk"))
        matches = rl_completion_matches((char*)text, &generate_talk);
    else if (!(wc == 1 && (first_word_eq(rl_line_buffer, "load") || first_word_eq(rl_line_buffer, "save"))))
        rl_bind_key('\t',rl_abort);
    
    return matches;
}
#endif /* USE_READLINE */

int main(int argc, char** argv)
{
    cmds["go"] = &go;
    cmds["look"] = &look;
    cmds["use"] = &use;
    cmds["pickup"] = &pickup;
    cmds["get"] = &pickup;
    cmds["equip"] = &equip;
    cmds["unequip"] = &unequip;
    cmds["drop"] = &drop;
    cmds["battle"] = &battle;
    cmds["talk"] = &talk;
    cmds["save"] = &save;
    cmds["load"] = &load_world;
    cmds["die"] = &die;
    // CHEAT CMD, remove later
    cmds["teleport"] = &teleport;
    cmds["i'mrich"] = &imrich;
    
    battleCmds["attack"] = &attack;
    battleCmds["use"] = &useBattle;
    battleCmds["run"] = &run;
    battleCmds["die"] = &die;
    battleCmds["kill"] = &kill;

    // shopCmds consists of the normal cmds plus a few extra, minus saving and loading
    shopCmds = cmds;
    shopCmds.erase("save");
    shopCmds.erase("load");
    shopCmds["list"] = &listItems;
    shopCmds["listinventory"] = &listItems;
    shopCmds["inventory"] = &listItems;
    shopCmds["buy"] = &buy;
    shopCmds["sell"] = &sell;
    
    ifstream ifs("file.txt");
    load(ifs);
    ifs.close();
    string str;

    #ifdef USE_READLINE
    char *inpt = nullptr;
    rl_attempted_completion_function = my_completion;
    while (nullptr != (inpt = readline("\n> ")))
    #else
    while (!cin.eof())
    #endif
    {
        try
        {
            #ifdef USE_READLINE
            rl_bind_key('\t', rl_complete);
            add_history(inpt);
            str = inpt;
            free(inpt);
            inpt = nullptr;
            #else
            cout << "\n> ";
            getline(cin, str);
            #endif
            
            boost::trim_all(str);
            size_t first_space = str.find_first_of(' ');
            string command = str.substr(0, first_space);
            string arg = (first_space == string::npos) ? "" : str.substr(first_space + 1);

            if (mode == BATTLE)
            {
                battleCmds.at(command)(arg);
                
                if (opponent != nullptr && opponent->isDead())
                {
                    player->addStats((opponent->getStats())*0.5);
                    delete opponent;
                    exitBattleMode();
                }
            }
            else if (mode == SHOP)
            {
                shopCmds.at(command)(arg);
            }
            else 
            {
                cmds.at(command)(arg);
            }

            if (player->isDead())
            {
                cout << "Game over" << endl;
                break;
            }

            if (winflag)
            {
                cout << "You beat the game!" << endl;
                break;
            }

            if (command != "look")                          // don't update on a look command
            {
                for (Room *room: Room::getRooms())
                    room->update();
            
                for (Actor *actor : Actor::getActors())
                    actor->update();
            }
        }
        catch(const out_of_range &e)
        {
            cout << "What are ye doing, LOL" << endl;
        }
        catch (const InvalidFileException &e)
        {
            cerr << "ERROR: Trying to load invalid file:" << endl;
            cerr << e.what() << endl;
            cerr << "ABNORMAL SHUTDOWN" << endl;
            return 1;
        }
        catch (const GameException &e)
        {
            cout << e.what() << endl;
        }
    }

    destroy_everything();
        
    return 0;
}
