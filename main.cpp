#include <string>
#include <stdio>
int main(int argc, char** argv)
{
    Room* kitchen = new Room("Kitchen", "This is the kitchen");
    Room* first = new Room("My room", "This is my room", "Kitchen", kitchen);
    Room* 
    char str[256];
    char* arg;
    Player* player;
    first->addActor(player);
    
    
    while(true)
    {
        fgets(str, 256, cin);
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
        
        player->go(arg);
        
    }
    
        
        
    return 0;
    
}
