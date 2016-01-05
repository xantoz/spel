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
    ItemOwner(name, description), dropItems(false), dead(false),
    armor(nullptr), shield(nullptr), sword(nullptr), shoes(nullptr)
{
}

Actor::Actor(const std::string &name, const std::string &description, const Stats &_stats) :
    ItemOwner(name, description), dropItems(false), stats(_stats), hp(_stats.maxhp), dead(false),
    armor(nullptr), shield(nullptr), sword(nullptr), shoes(nullptr)
{
}

Actor::Actor(const std::string &name, const std::string &description, const Stats &_stats, int _hp) :
    ItemOwner(name, description), dropItems(false), stats(_stats), hp(_hp), dead(false),
    armor(nullptr), shield(nullptr), sword(nullptr), shoes(nullptr)
{
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
}


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

std::string Actor::use(const std::string &itemName)
{
    Item *item = this->getItem(itemName);
    if (item == nullptr)
        throw NoSuchItemException();
    return item->use();
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
    int damage;
    if(stats.def != 0)
    {
        damage = (int) (2*atk/(double)stats.def);
        hp -= damage;
        //std::cerr << "beAttacked if" << std::endl;
        
    }
    else 
    {
        damage = hp;
        hp = 0;
        //std::cerr << "beAttacked else" << std::endl;
        
    }
    std::cout << getName() << " took " << damage << " damage." << std::endl;
    
    if(hp <= 0)
        die();
    else 
    {
        attackResponse(actor);
    }
}

void Actor::attackResponse(Actor *actor)
{
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
        for (Item *item: this->getItems())
        {
            std::cout << getName() << " dropped " << item->getName() << std::endl;
            room->addItem(item);
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
    this->serializeItems(os, actorSym);
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

    // for (auto const &ent: deathExits)
    // {
    //     os << ":SET-DEATH-EXIT " << actorSym << " \"" << ent.first << " " << 
    // }
}

int Actor::getHp() const
{
    return hp;
}

void Actor::setHp(int hphp)
{
    hp = hphp;
}
