#include "Actor.hpp"
#include "Room.hpp"
#include "exceptions.hpp"
#include "Serialize.hpp"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <sstream>
// IMPORTANT INFORMATION: Before you can delete any Item that Actor has equipped it has to be
// unequipped. Item's destructor will otherwise explode.

Actor::Actor(const std::string &name, const std::string &description) :
    ItemOwner(name, description), room(nullptr), dropItems(false), dead(false),
    armor(nullptr), shield(nullptr), sword(nullptr), shoes(nullptr)
{
    actors.push_front(this);
}

Actor::Actor(const std::string &name, const std::string &description, const Stats &_stats) :
    ItemOwner(name, description), room(nullptr), dropItems(false), stats(_stats), hp(_stats.maxhp), dead(false),
    armor(nullptr), shield(nullptr), sword(nullptr), shoes(nullptr)
{
    actors.push_front(this);
}

Actor::Actor(const std::string &name, const std::string &description, const Stats &_stats, int _hp) :
    ItemOwner(name, description), room(nullptr), dropItems(false), stats(_stats), hp(_hp), dead(false),
    armor(nullptr), shield(nullptr), sword(nullptr), shoes(nullptr)
{
    actors.push_front(this);
}


Actor::~Actor()
{
    std::cerr << "Actor<" << getName() << "> destructor" << std::endl;
    
    if (room != nullptr)
        room->removeActor(this);

    // Make sure to unequip everything so that the ItemOwner destructor will find everything in  items as it expects
    unequipSword();
    unequipShield();
    unequipArmor();
    unequipShoes();
    actors.remove(this);
}
std::list<Actor*> Actor::actors;

const std::list<Actor*>& Actor::getActors()
{
    return Actor::actors;
}


void Actor::update() {}


bool Actor::equipArmor(const std::string &name)
{
    Armor *armor = dynamic_cast<Armor*>(getItem(name));
    if (nullptr == armor) return false; // Either doesn't exist or isn't an armor
    unequipArmor();                     // ensure armor slot is empty
    items.erase(std::find(items.begin(), items.end(), armor));
    this->armor = armor;
    return true;
}

bool Actor::equipShield(const std::string &name)
{
    Shield *shield = dynamic_cast<Shield*>(this->getItem(name));
    if (nullptr == shield) return false; // Either doesn't exist or isn't a shield
    unequipShield();                     // ensure shield slot is empty
    items.erase(std::find(items.begin(), items.end(), shield));
    this->shield = shield;
    return true;
}

bool Actor::equipSword(const std::string &name)
{
    Sword *sword = dynamic_cast<Sword*>(this->getItem(name));
    if (nullptr == sword) return false;                 // Either doesn't exist or isn't a sword
    unequipSword();                                     // make sure sword slot is empty
    items.erase(std::find(items.begin(), items.end(), sword));
    this->sword = sword;
    return true;
}

bool Actor::equipShoes(const std::string &name)
{
    Shoes *shoes = dynamic_cast<Shoes*>(this->getItem(name));
    if (nullptr == shoes) return false; // Either doesn't exist or isn't a pair of shoes
    unequipShoes();                     // ensure shoes slot is empty
    items.erase(std::find(items.begin(), items.end(), shoes));
    this->shoes = shoes;
    return true;
}

void Actor::unequipArmor()
{
    if (armor == nullptr) return;
    items.push_front(armor);
    armor = nullptr;
}

void Actor::unequipShield()
{
    if (shield == nullptr) return;
    items.push_front(shield);
    shield = nullptr;
}

void Actor::unequipSword()
{
    if (sword == nullptr) return;
    items.push_front(sword);
    sword = nullptr;
}

void Actor::unequipShoes()
{
    if (shoes == nullptr) return;
    items.push_front(shoes);
    shoes = nullptr;
}

const Armor *Actor::getArmor() const { return armor; }
const Shield *Actor::getShield() const { return shield; }
const Sword *Actor::getSword() const { return sword; }
const Shoes *Actor::getShoes() const { return shoes; }

void Actor::go(std::string direction)
{
    Room *nroom = room->getExit(direction);
    if (nroom == nullptr)
        throw NoSuchExitException();
    nroom->addActor(this); // this moves the actor to nroom (sets this->room)
}

void Actor::use(const std::string &itemName)
{
    Item *item = this->getItem(itemName);
    if (item == nullptr)
        throw NoSuchItemException();
    item->use(this);
}

void Actor::use(const std::string &itemName,
                       const std::string &useOn)
{
    Item *item = this->getItem(itemName);
    if (item == nullptr)
        throw NoSuchItemException();
    Actor* actor = getRoom()->getActor(useOn);
    if(actor == nullptr)
        throw NoSuchActorException();
    item->use(actor);
}

bool Actor::pickup(const std::string &itemName)
{
    if (room == nullptr) return false;
    Item *i = room->getItem(itemName);
    if (i == nullptr) return false;
    bool res = this->addItem(i);
    if (res == false)
        std::cout << "Can't carry any more" << std::endl;
    return res;
}

bool Actor::drop(const std::string &itemName)
{
    if (room == nullptr) return false;
    Item *i = this->getItem(itemName);
    if (i == nullptr) return false;
    return room->addItem(i);
}

Room *Actor::getRoom() const
{
    return room;
}

const Stats &Actor::getStats() const
{
    return stats;
}

void Actor::setStats(const Stats &newstats)
{
    stats = newstats;
}

std::string Actor::getDescription() const
{
    std::ostringstream ret;
    ret << "Name: " << getName() << "\n";
    ret << "HP: " << hp << "/" << getStats().maxhp << "\n\n";
    ret << getBaseDescription() << "\n\n";
    ret << "STATS: " << getTotalStats().toString() << "\n";
    ret << "INVENTORY:";
    if (items.size() > 0)
    {
        auto it = items.begin();
        ret << (*it)->getName();
        ++it;
        for (; it != items.end(); ++it)
            ret << ", " << (*it)->getName();
    }
    
    else
        ret << " EMPTY";
    ret << "\n";
    ret << "SWORD: "   << ((sword)  ? sword->getName()  : "NONE")
        << " SHIELD: " << ((shield) ? shield->getName() : "NONE")
        << " ARMOR: "  << ((armor)  ? armor->getName()  : "NONE")
        << " SHOES: "  << ((shoes)  ? shoes->getName()  : "NONE");
    return ret.str();
}

Stats Actor::getTotalStats() const 
{
    Stats tot = stats;
    if(sword != nullptr)
        tot += sword->getStats();
    if(shield != nullptr)
        tot += shield->getStats();
    if(armor != nullptr)
        tot += armor->getStats();
    if(shoes != nullptr)
        tot += shoes->getStats();
    return tot;
}


bool Actor::attack(const std::string &actorName)
{
    Actor* actor = room->getActor(actorName);
    if (actor == nullptr)
        return false;
    attack(actor);
    return true;
}

void Actor::attack(Actor *actor)
{
    Stats stats = getTotalStats();
    double rval = (std::rand()/(double) RAND_MAX)*(1.2-0.8)+0.8;
    unsigned int atk = (unsigned int)(stats.atk*rval);
    actor->beAttacked(this, atk);
}

void Actor::beAttacked(Actor *actor, unsigned int atk)
{
    Stats stats = getTotalStats();
    Stats aStats = actor->getTotalStats();
    double quote = (aStats.acc+1)/(double)(stats.eva+1);
    //std::cout << aStats.acc << std::endl;
    //std::cout << stats.eva << std::endl;
    
    //std::cout << quote << std::endl;

    int damage;
     if(std::rand()/(double)RAND_MAX <= quote)
     {
         if(stats.def != 0)
         {
             damage = (int) (2*atk/(double)stats.def);
             hp -= damage;
         }
         else 
         {
             damage = hp;
             hp = 0;
         }
         std::cout << getName() << " took " << damage << " damage." << std::endl;
     }
     else
     {
         std::cout << actor->getName() << "'s attack missed!" << std::endl;
     }
    
     if(hp <= 0)
         die();
     else 
     {
         attackResponse(actor);
     }
}

void Actor::attackResponse(Actor *actor)
{
    std::cout << getName() << "'s HP: " << hp << "/" << stats.maxhp << std::endl;
    attack(actor);
}

void Actor::die()
{
    std::cout << getName() << " died." << std::endl;

    for (auto &ent: deathExits)
    {
        room->setExit(ent.first, ent.second);
        std::cout << "A door opened to the " << ent.first << std::endl;
    }

    if (dropItems && room != nullptr)
    {
        unequipArmor();
        unequipShield();
        unequipSword();
        unequipShoes();
        for (auto it = getItems().begin(); it != getItems().end(); )
        {
            std::cout << getName() << " dropped " << (*it)->getName() << std::endl;
            // Ensure we have a reference to the iterator one step after the one that'll be
            // removed (moved to rooms list of items)
            room->addItem(*it++); 
        }
    }
    
    dead = true;
}

bool Actor::isDead()
{
    return dead;
}

void Actor::setDeathExit(const std::string &name, Room* room)
{
    deathExits[name] = room;
}

const std::map<std::string, Room*> &Actor::getDeathExits() const
{
    return deathExits;
}

void Actor::removeDeathExit(const std::string &direction)
{
    if (deathExits.find(direction) == deathExits.end())
        throw NoSuchExitException(); // TODO: possibly more severe than regular NoSuchExitException
    deathExits.erase(direction);
}

void Actor::removeDeathExit(Room *exit)
{
    std::map<std::string, Room*>::iterator
        it = std::find_if(deathExits.begin(), deathExits.end(),
                          [&](const std::pair<std::string, Room*> &item) -> bool { return item.second == room; });
    if (it == deathExits.end())
        throw NoSuchExitException(); // TODO: possibly more severe than regular NoSuchExitException
    deathExits.erase(it);
}

void Actor::setDrop(bool drop)
{
    dropItems = drop;
}

std::string Actor::serialize(std::ostream &os) const
{
    std::string actorSym = gensym();
    os << actorSym << ":MAKE-ACTOR ";
    actorTypeIndependentSerializeConstructorParameters(os);
    actorTypeIndependentSerialize(os, actorSym);
    return actorSym;
}

void Actor::actorTypeIndependentSerializeConstructorParameters(std::ostream &os) const
{
    os << stringify(getName()) << " " << stringify(getBaseDescription()) << " " << stats.serializeString() << " " << hp << std::endl;
    // os << "\"" << getName() << "\" \"" << getBaseDescription() << "\"" << " " << stats.serializeString() << " " << hp << std::endl;
}


void Actor::actorTypeIndependentSerialize(std::ostream &os, const std::string &actorSym) const
{
    // this->serializeItems(os, actorSym);
    if (sword != nullptr)
    {
        std::string itemSym = sword->serialize(os);
        os << ":ADD-ITEM " << actorSym << " " << itemSym << std::endl;
        os << ":EQUIP-SWORD " << actorSym << " " << stringify(sword->getName()) << std::endl;
        
    }
    if (armor != nullptr)
    {
        std::string itemSym = armor->serialize(os);
        os << ":ADD-ITEM " << actorSym << " " << itemSym << std::endl;
        os << ":EQUIP-ARMOR " << actorSym << " " << stringify(armor->getName()) << std::endl;
    }
    if (shield != nullptr)
    {
        std::string itemSym = shield->serialize(os);
        os << ":ADD-ITEM " << actorSym << " " << itemSym << std::endl;
        os << ":EQUIP-SHIELD " << actorSym << " " << stringify(shield->getName()) << std::endl;
    }
    if (shoes != nullptr)
    {
        std::string itemSym = shoes->serialize(os);
        os << ":ADD-ITEM " << actorSym << " " << itemSym << std::endl;
        os << ":EQUIP-SHOES " << actorSym << " " << stringify(shoes->getName()) << std::endl;
    }

    os << ":SET-DROP " << actorSym << " " << dropItems << std::endl;

    // The rest of the Items owned by actor (currently in inventory) is taken care of in the
    // ItemOwner pass in serialize after we're certain all rooms have been created.
    // The death-exits are also taken care of in a special pass in serialize after we can be
    // sure all rooms have been created.
}

int Actor::getHP() const
{
    return hp;
}

void Actor::setHP(int hphp)
{
    hp = hphp;
}
