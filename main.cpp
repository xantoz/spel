#include "Room.hpp"
#include "Player.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    Room* kitchen = new Room("Kitchen", "This is the kitchen");
    Room* first = new Room("My room", "This is my room", "Kitchen", kitchen);
    char str[256];
    char* arg;
    Player* player = new Player();;
    first->addActor(player);

    cout << player->getRoom()->getName() << endl;
    cout << player->getRoom()->getDescription() << endl;
    while(true)
    {
        fgets(str, 256, stdin);
        arg = NULL;
        for(char* c = str; *c !='\0'; ++c)
        {
            if(*c == ' ')
            {
                *c = 0;
                arg = c+1;
                break;
            }

        }

        if (strcmp(str, "go") == 0)
        {
            player->go(arg);
            cout << player->getRoom()->getName() << endl;
            cout << player->getRoom()->getDescription() << endl;
        }
        else
        {
            cout << "What are ye doing, LOL" << endl;
        }
    }
    
        
        
    return 0;
    
}
