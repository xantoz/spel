#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>


class ItemOwner;
class Room;


class Item
{
private:
    ItemOwner *owner;
    std::string name;
    std::string description;
    unsigned weight;
public:

    Item(std::string nam, std::string descriptio, unsigned weigh) : owner(nullptr), name(nam), description(descriptio), weight(weigh)
    {
    }

    const std::string& getName() const
    {
        return name;
    }

    const std::string& getDescription() const
    {
        return description;
    }

    unsigned getWeight() const
    {
        return weight;
    }

    friend class ItemOwner;
};


class ItemOwner 
{
protected:
    virtual bool can_carry(const Item* item) const
    {
        return true;
    }
    
    std::list<Item*> items;

public:
    bool add_item(Item *i)
    {
        // TODO: or use exception
        if (!this->can_carry(i))
            return false;
                
        items.push_front(i);
        if (i->owner != nullptr) // items can be ownerless (though normally only directly after being constructed
            i->owner->removeItem(i);
        i->owner = this;

        return true;
    }
    
    void removeItem(Item* i)
    {
        // TODO: instead of returning bool throw exception when it doesn't exist
        
        auto it = std::find(items.begin(), items.end(), i);
        if (it == items.end())
        {
            // TODO: THROW EXCEPTION
        }
        items.erase(it);
    }

    virtual ~ItemOwner()
    {
        std::cerr << "ItemOwner destructor" << std::endl;
        
        for (Item *i: items)
            delete i;
    }
};

class Actor : public ItemOwner 
{
private:
    Room *room;
    
public:
    bool go(std::string direction)
    {
        // TODO: use exceptions instead of returning false?
        
        Room *nroom = room->getExit(direction);
        if (nroom == nullptr)
            return false;
        room->removeActor(this);
        nroom->addActor(this);                              // this sets room to nroom
        // room = nroom;

        return true;
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

    friend void main_loop();
};


class Player : public Actor
{
private:
    static unsigned magisk_konstant = 10;
protected:
    virtual bool can_carry(const Item *i) const override
    {
        unsigned total_weight = 0;
        for(Item *i: items)
            total_weight += i->getWeight();
        total_weight += i->getWeight();
        return (total_weight > strength*magisk_konstant);
    }
private:
    unsigned strength;
public:
    
};





// void asdf()
// {
//     Player *p = new Player("nisse");
//     p->add_item(new Item("dagger", "pointy", 20));
    
//     Room *room = new Room("castle", "dark");
//     room->add_item(new Item("cloak", "flowy", 30));
// }

