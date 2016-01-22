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

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <functional>
#include <boost/algorithm/string/trim_all.hpp>

using namespace std;

enum Mode 
{
    NORMAL, BATTLE, SHOP
} mode;

Player *player;
Actor *opponent;
Shop *shop;

void destroy_everything()
{
    while (Room::getRooms().size() > 0)
    {
        Room *room = Room::getRooms().front();
        cout << "==Deleting room " << room->getName() << "==" << endl;
        delete room;
    }

    player = nullptr;
    opponent = nullptr;
    mode = NORMAL;
}

void enterBattleMode(Actor *actor)
{
    mode = BATTLE;
    opponent = actor;
    cout << "Entered battle with " << opponent->getName() << endl;
    cout << "Opponent has Stats: " << opponent->getStats().toString() << endl;
}

void exitBattleMode()
{   
    mode = NORMAL;
    opponent = nullptr;
    cout << "Exited battle" << endl;
}

void go(string arg)
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

void die(string arg)
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

void look(string arg)
{
    if (arg == "")
        cout << player->look() << endl;
    else
        cout << player->look(arg) << endl;    
}

void use(string arg)
{
    if (arg == "")
    {
        cout << "What do you want to use?" << endl;
        return;
    }
    else 
    {
        auto it = arg.begin();
        for(; it != arg.end() && *it != ' '; ++it);
        string first(arg.begin(), it);
        for(; it != arg.end() && *it == ' '; ++it);
        string second(it, arg.end());
        if(second == "")
            player->use(first);
        else
            player->use(first, second);

        // always non-null since we'd have triggered an exception with player->use for a non-existant item
        Item *item = player->getItem(first);
        if (item->usedUp())
        {
            std::cout << "You used up the " << item->getName() << std::endl;
            delete item;
        }

    }
    
}

void useBattle(string arg)
{
    if (arg == "")
    {
        cout << "What do you want to use?" << endl;
        return;
    }
    else 
    {
        auto it = arg.begin();
        for(; it != arg.end() && *it != ' '; ++it);
        string first(arg.begin(), it);
        for(; it != arg.end() && *it == ' '; ++it);
        string second(it, arg.end());
        if(second == "")
            player->use(first);
        else
        {
            player->use(first, second);
        }

        opponent->attackResponse(player);

        // always non-null since we'd have triggered an exception with player->use for a
        // non-existant item
        
        Item *item = player->getItem(first);
        if (item->usedUp())
            delete item;

    }
}


void pickup(string arg)
{
     if (arg == "")
     {
         cout << "What did you want to pick up?" << endl;
         return;
     }
     if (player->pickup(arg))
         cout << "Picked up " << arg << endl;
     else
         cout << "I don't see any such thing." << endl;
}

void drop(string arg)
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

void unequip(string arg)
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

void battle(string arg)
{
    if(arg == "")
        cout << "Battle who?" << endl;
    else 
    {
        Actor *actor = player->getRoom()->getActor(arg);
        if(actor != nullptr)
        {
            enterBattleMode(actor);
        }
    }
}


void attack(string arg)
{

    player->attack(opponent);
}

void run(string arg)
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

void talk(string arg)
{
    if(arg == "")
        cout << "Talk who?" << endl;
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

// template <typename I>
// I random_element(I begin, I end)
// {
//     const unsigned long n = std::distance(begin, end);
//     const unsigned long divisor = (RAND_MAX + 1) / n;

//     unsigned long k;
//     do { k = std::rand() / divisor; } while (k >= n);

//     return std::advance(begin, k);
// }

// void randomSpawn()
// {
//     static unsigned cntr = 0;
//     if (rand() % 5 == 1)
//     {
//         Actor *act = new Thief("Thief" + std::to_string(++cntr), rand() % 3);
//         auto rooms = Room::getRooms();
//         // (*random_element(rooms.begin(), rooms.end()))->addActor(act);
                
//         unsigned cnt = rand() % rooms.size();
//         auto it = rooms.begin();
//         for (unsigned i = 0; i < cnt; ++i)
//             ++it;
//         (*it)->addActor(act);
//     }
// }

void save(string filename)
{
    if (filename == "")
        filename = "quicksave.save";
    std::ofstream outfile(filename);
    serialize(Room::getRooms(), outfile);
    cout << "Saved world to " << filename << endl;
}

void load_world(string filename)
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
void listItems(string arg)
{
    cout << shop->listInventory();
}

void buy(string arg)
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
                shop->removeShopItem(item);
                player->addMoney(-item.second);
                player->addItem(item.first);
                cout << "You bought the " << item.first->getName()  << "!" << endl;
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

void sell(string arg)
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
            cout << "You sold the item for " << item->getWeight() << endl;
            player->removeItem(item);
        }    
    }       
}




int main(int argc, char** argv)
{
    std::map<string, function<void(string)>> cmds;
    std::map<string, function<void(string)>> battleCmds;
    std::map<string, function<void(string)>> shopCmds;
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
    
    battleCmds["attack"] = &attack;
    battleCmds["use"] = &useBattle;
    battleCmds["run"] = &run;
    battleCmds["die"] = &die;

    shopCmds["list"] = &listItems;
    shopCmds["buy"] = &buy;
    shopCmds["sell"] = &sell;
    shopCmds["go"] = &go;
    
    Room* kitchen = new Room("Kitchen", "This is the kitchen", nullptr);
    Room* secret = new Room("Secret Room", "Actually this is just your wardrobe.", EncounterProbability(0.5, 0.3, 0.4, 0.6));
    Room* first = new Room("My room", "This is my room\n You see a yellow locked door to the east.", "west", kitchen, nullptr);
    first->addItem(new Key("yellowkey", "A yellow key", 5, first, "east", secret, "west"));
    auto *widget = new CallbackItem("widget", "A magical talking widget", 4, "widget.script");
    widget->setConsumable(true);
    first->addItem(widget);
    Room* outside = new Room("Outside", "This is our garden", "south", kitchen, nullptr);
    Room* neighbor = new Room("Neighbor", "Outside of neighbors house", "east", outside, nullptr);
    Room* street = new Room("Street", "The street", "northwest", neighbor, "northeast", outside,nullptr);
    Room* park = new Room("Park", "The biggest park in this city. \nThere's a big tree in the middle", "south", street,nullptr);
    outside->setExit("west", neighbor);
    outside->setExit("northwest", street);
    neighbor->setExit("northeast", street);
    street->setExit("north", park);

    std::list<std::pair<Item*, unsigned>> shop1List;
    Stats goldshieldstats = {0, 0, 40, -3, 0, -2};
    Stats golddaggerstats = {0, 0, 30, 0, 0, 1, 4};
            
    shop1List.push_back(std::pair<Item*, unsigned>(new Shield("Gold shield", "An awesome shield", 40, goldshieldstats), 90));
    shop1List.push_back(std::pair<Item*, unsigned>(new Sword("Gold dagger", "An awesome dagger", 50, golddaggerstats), 100));
    shop1List.push_back(std::pair<Item*, unsigned>(new Potion("GoldPotion", 4), 30));                              
    Shop* shop = new Shop("Shop", "The shop, type look to list all items", shop1List );
    kitchen->setExit("north", outside);
    kitchen->setExit("east", first);
    neighbor->setExit("shop", shop);
    shop->setExit("exit", neighbor);
    
    Stats shieldstats = {0, 0, 0, 23, -2, 0, -1};
    first->addItem(new Shield("bronze shield", "a typical shield", 20, shieldstats));
    Stats daggerstats = {0, 0, 10, 0, 0, 1, 2};
    kitchen->addItem(new Sword("dagger", "It's just your normal dagger.", 10, daggerstats));
    kitchen->addItem(new Potion("Potion1", 2));
    
    Stats oldmanstats = {50, 10, 20, 5, 10, 10};        
    Human* oldman = new Human("Rudolph", "It's your uncle", oldmanstats, "Good morning my son, where are you going?");
    outside ->addActor(oldman);

    Stats nilsstats = { 20, 10, 10, 10, 10, 10, 20 };
    player = new Player("nils", "it's you", nilsstats);
    first->addActor(player);
    Troll* troll = new Troll("Troll1", 2);
    kitchen->addActor(troll);
    Stats bigTreeStats = {1000, 10, 100, 10, 10, 10, 1};

    Stats axestats = {0, 0, 20, 0, -10,-5,-5};
    neighbor->addItem(new Sword("Axe", "A really heavy axe", 100, axestats));
    
    Actor* yTree = new Actor("BigTree", "The tree looks really old", bigTreeStats);
    park->addActor(yTree);
    cout << player->getRoom()->getName() << endl;
    cout << player->getRoom()->getDescription() << endl;
    player->addMoney(100);
    
    string str;
    while(!cin.eof())
    {
        try
        {
            if(player->isDead())
            {
                cout << "Game over" << endl;
                return 0;
            }
            vector<Actor*> actorList;
            for(Room *room : Room::getRooms())
            {
                for(Actor *actor : room->getActors())
                {
                    actorList.push_back(actor);
                }
                room->update();
            }
            for(Actor *actor : actorList)
            {
                actor->update();
            }
            // randomSpawn();
            
            cout << "\n> ";
            getline(cin, str);
            boost::trim_all(str);
            size_t first_space = str.find_first_of(' ');
            string command = str.substr(0, first_space);
            string arg = (first_space == string::npos) ? "" : str.substr(first_space + 1);
            
            if (mode == BATTLE)
            {
                battleCmds.at(command)(arg);
                
                if (opponent != nullptr && opponent->isDead())
                {
                    player->addStats((opponent->getStats())*0.1);
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
            // cout << "\"" << command << "\" \"" << arg << "\"" << endl;
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
