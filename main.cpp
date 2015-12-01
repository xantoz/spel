#include "Room.hpp"
#include "Player.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    Room* kitchen = new Room("Kitchen", "This is the kitchen", nullptr);
    Room* first = new Room("My room", "This is my room", "Kitchen", kitchen, nullptr);
    std::string str;
    Player* player = new Player("nils", "it's you");
    first->addActor(player);

    cout << player->getRoom()->getName() << endl;
    cout << player->getRoom()->getDescription() << endl;
    while(true)
    {
        std::getline(cin,str);
        size_t space_index = str.find_first_of(' ');
        size_t first_non_space_index = str.find_first_not_of(' ');
        std::string command = str.substr(first_non_space_index, space_index);
        first_non_space_index = str.find_first_not_of(' ', space_index);
        std::string arg = str.substr(first_non_space_index,str.length());

        if (command == "go")
        {
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
        }
        
        else
        {
            cout << "What are ye doing, LOL" << endl;
        }
    }
    
        
        
    return 0;
    
}
