#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>


class ItemOwner;
class Room;




class Actor : public ItemOwner 
{
private:
    Room *room;
    
public:
    void go(std::string direction)
    {
        // TODO: use exceptions instead of returning false?
        
        Room *nroom = room->getExit(direction);
        if (nroom == nullptr)
        {
            // TODO: throw exception
        }
        room->removeActor(this);
        nroom->addActor(this);                              // this sets room to nroom
    }

    Room *getRoom()
    {
        return room;
    }

    friend class Room;
};


class Room : public ItemOwner 
{
private:
    std::string name;
    std::string description;
    
    std::map<std::string, Room*> exits;
    std::list<Actor*> actors;
public:
    Room *getExit(std::string direction) const
    {
        // TODO: or use exceptions instead to signal non-existant exits

        if (exits.find(direction) == exits.end())
            return nullptr;
        return exits.at(direction);
    }

    void addActor(Actor *a)
    {
        actors.push_front(a);
        // we're friends with the actors guild
        if (a->room == nullptr)
            a->room->remove_actor(a);
        a->room = this;
    }

    void removeActor(Actor *a)
    {
        auto it = std::find(actors.begin(), actors.end(), a);
        if (it == actors.end())
        {
            // TODO: THROW EXCEPTION
        }
        actors.erase(it);
        return true;
    }

    const std::list<Actor*> &getActors()
    {
        return actors;
    }

    // friend void main_loop();
};


class Player : public Actor
{
private:
    static unsigned magisk_konstant = 10;
    unsigned strength;
protected:
    virtual bool can_carry(const Item *i) const override
    {
        unsigned total_weight = 0;
        for(Item *i: items)
            total_weight += i->getWeight();
        total_weight += i->getWeight();
        return (total_weight > strength*magisk_konstant);
    }
public:
    
};





// void asdf()
// {
//     Player *p = new Player("nisse");
//     p->add_item(new Item("dagger", "pointy", 20));
    
//     Room *room = new Room("castle", "dark");
//     room->add_item(new Item("cloak", "flowy", 30));
// }

