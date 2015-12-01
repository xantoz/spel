#include "exceptions.hpp"
#include "Room.hpp"
#include "Player.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    Room* kitchen = new Room("Kitchen", "This is the kitchen", nullptr);
    Room* first = new Room("My room", "This is my room", "west", kitchen, nullptr);
    kitchen->setExit("east", first);
    
    Player* player = new Player("nils", "it's you");
    first->addActor(player);

    cout << player->getRoom()->getName() << endl;
    cout << player->getRoom()->getDescription() << endl;
    std::string str;
    while(!cin.eof())
    {
        try 
        {
            cout << "\n> ";
            std::getline(cin,str);
            size_t first_non_space_index = str.find_first_not_of(' ');
            first_non_space_index = (first_non_space_index != string::npos) ? first_non_space_index : 0;
            size_t space_index = str.find_first_of(' ', first_non_space_index);
            space_index = (space_index != string::npos) ? space_index : str.length();
            std::string command = str.substr(first_non_space_index, space_index);
            std::string arg = "";
            if (space_index != str.length())
            {
                size_t arg_first_non_space_index = str.find_first_not_of(' ', space_index);
                arg = str.substr(arg_first_non_space_index, str.length());
            }

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
            else if(command == "look")
            {
                cout << player->getRoom()->getDescription() << endl;
            }
            else if(command == "use")
            {
                if (arg == "")
                {
                    cout << "What do you want to use?" << endl;
                    continue;
                }
                cout << player->use(arg) << endl;
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
    
        
        
    return 0;
    
}
