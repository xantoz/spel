#include "exceptions.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Sword.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

#include <boost/algorithm/string/trim_all.hpp>

using namespace std;

int main(int argc, char** argv)
{
    Room* kitchen = new Room("Kitchen", "This is the kitchen", nullptr);
    Room* first = new Room("My room", "This is my room", "west", kitchen, nullptr);
    kitchen->setExit("east", first);
    Stats daggerstats = {0, 0, 10, 0, 0, 1, 2};
    kitchen->addItem(new Sword("dagger", "It's just your normal dagger.", 10, daggerstats));

    Stats nilsstats = { 20, 10, 10, 10, 10, 10 };
    Player* player = new Player("nils", "it's you", nilsstats);
    first->addActor(player);

    cout << player->getRoom()->getName() << endl;
    cout << player->getRoom()->getDescription() << endl;
    std::string str;
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

            // cout << "\"" << command << "\" \"" << arg << "\"" << endl;

            if (command == "go")
            {
                if (arg == "")
                {
                    cout << "Where did you want to go, you said?" << endl;
                    continue;
                }
                player->go(arg);
                cout << player->getRoom()->getName() << endl;
                cout << player->getRoom()->getDescription() << endl;
            }
            else if (command == "look")
            {
                // cout << player->getRoom()->getDescription() << endl;
                if (arg == "")
                    cout << player->look() << endl;
                else
                    cout << player->look(arg) << endl;
            }
            else if (command == "use")
            {
                if (arg == "")
                {
                    cout << "What do you want to use?" << endl;
                    continue;
                }
                cout << player->use(arg) << endl;
            }
            else if (command == "pickup")
            {
                if (arg == "")
                {
                    cout << "What did you want to pick up?" << endl;
                    continue;
                }
                if (player->pickup(arg))
                    cout << "Picked up " << arg << endl;
                else 
                    cout << "I don't see any such thing." << endl;
            }
            else if (command == "drop")
            {
                if (arg == "")
                {
                    cout << "What did you want to drop?" << endl;
                    continue;
                }
                if (player->drop(arg))
                    cout << "Dropped " << arg << endl;
                else
                    cout << "I'm not carrying anything like that." << endl;
            }
            else
            {
                cout << "What are ye doing, LOL" << endl;
            }
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
