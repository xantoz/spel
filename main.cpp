#include "exceptions.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Sword.hpp"

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <functional>
#include <boost/algorithm/string/trim_all.hpp>

using namespace std;
Player *player;
Actor *opponent;
bool battleMode;

void enterBattleMode(Actor *actor)
{
    battleMode = true;
    opponent = actor;
    cout << "Entered battle with " << opponent->getName() << endl;
    
}

void exitBattleMode()
{
    battleMode = false;
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
}

void look(string arg)
{
 // cout << player->getRoom()->getDescription() << endl
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
    cout << player->use(arg) << endl;   
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

    cerr << "meep meep" << endl;
    if (player->getItem(what))
        cerr << "BLOOOORP " << typeid(*player->getItem(what)).name() << endl;
    
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
            enterBattleMode(arg);
        }
    }
}


void attack(string arg)
{
    if(arg == "")
    {
        list<Actor*> actors = player->getRoom()->getActors();
        player->attack(actors.getFirst());
    }
    else 
    {
        player->attack(arg);
    }
}

void use(string arg)
{
    if (arg == "")
    {
        cout << "What do you want to use?" << endl;
        return;
    }
    cout << player->use(arg) << endl; 
}


void run(string arg)
{
    if (opponent->getTotalStats().spd > player->getTotalStats().sped)
    {
        int rval = std::rand()/(double)RAND_MAX;
        if (rval > 0.25)
        {
            exitBattleMode();
        }
    }
    exitBattleMode();
}    


int main(int argc, char** argv)
{
    std::map<string, function<void(string)> > cmds;
    std::map<string, function<void(string)>> battleCmds;
    
    cmds["go"] = &go;
    cmds["look"] = &look;
    cmds["use"] = &use;
    cmds["pickup"] = &pickup;
    cmds["get"] = &pickup;
    cmds["equip"] = &equip;
    cmds["unequip"] = &unequip;
    cmds["drop"] = &drop;
    cmds["battle"] = &battle;

    battleCmds["attack"];
    battleCmds["use"];
    battleCmds["run"];
    
    Room* kitchen = new Room("Kitchen", "This is the kitchen", nullptr);
    Room* first = new Room("My room", "This is my room", "west", kitchen, nullptr);
    kitchen->setExit("east", first);
    
    Stats shieldstats = {0, 0, 0, 23, -2, 0, -1};
    first->addItem(new Shield("bronze shield", "a typical shield", 20, shieldstats));
    Stats daggerstats = {0, 0, 10, 0, 0, 1, 2};
    kitchen->addItem(new Sword("dagger", "It's just your normal dagger.", 10, daggerstats));

    Stats nilsstats = { 20, 10, 10, 10, 10, 10 };
    player = new Player("nils", "it's you", nilsstats);
    first->addActor(player);

    cout << player->getRoom()->getName() << endl;
    cout << player->getRoom()->getDescription() << endl;
    string str;
    while(!cin.eof())
    {
        try 
        {
            cout << "\n> ";
            getline(cin, str);
            boost::trim_all(str);

            size_t first_space = str.find_first_of(' ');
            string command = str.substr(0, first_space);
            string arg = (first_space == string::npos) ? "" : str.substr(first_space + 1);
            if(battleMode)
            {
                battleCmds.at(command)(arg);
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
        catch (const GameException &e)
        {
            cout << e.what() << endl;
        }
    }

    delete kitchen;
    delete first;
        
    return 0;
    
}
