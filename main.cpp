#include "exceptions.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Sword.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>
#include <functional>
#include <boost/algorithm/string/trim_all.hpp>

using namespace std;
Player* player;

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
    if (type == "sword")
    {
        player->equipSword(what);
    }
    else if ("shield" == type)
    {
        player->equipShield(what);
    }
    else if ("shoes" == type)
    {
        player->equipShoes(what);
    }
    else if ("armor" == type)
    {
        player->equipArmor(what);
    }
    else
    {
        cout << "There's no " << type << " kind of equipment in this world." << endl;
        return;
    }

    cout << "Equipped " << arg << "." << endl;
}

int main(int argc, char** argv)
{
    std::map<string, function<void(string)> > map;
    map["go"] = &go;
    map["look"] = &look;
    map["use"] = &use;
    map["pickup"] = &pickup;
    map["get"] = &pickup;
    map["equip"] = &equip;
    map["drop"] = &drop;
    
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
            map.at(command)(arg);
            
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
