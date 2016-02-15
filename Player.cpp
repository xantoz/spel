#include "Player.hpp"
#include "Room.hpp"
#include "Serialize.hpp"
#include <iostream>

// Player::Player(const std::string &name, const std::string &description) :
//     Actor(name, description)
// {
// }

Player::Player(const std::string &name, const std::string &description, const Stats &stats) :
    Player(name, description, stats, stats.maxhp, 0)
{
}

Player::Player(const std::string &name, const std::string &description, const Stats &stats, int hp) :
    Player(name, description, stats, hp, 0)
{
}

Player::Player(const std::string &name, const std::string &description, const Stats &stats, int hp, int dough) :
    Actor(name, description, stats, hp), kills(0), money(dough)
{
}

Player::~Player()
{
    std::cerr << "Player<" << getName() << "> destructor" << std::endl;
}

unsigned Player::max_carry() const
{
    return stats.str*magisk_konstant;
}

bool Player::can_carry(const Item *i) const
{
    return (getEncumberment() < max_carry());
}

std::string Player::getDescription() const 
{
    std::string firstPart = Actor::getDescription();
    return firstPart
        + "\nMoney: " + std::to_string(money)
        + "\nEncumberment: " + std::to_string(getEncumberment()) + "/" + std::to_string(max_carry());
}


std::string Player::look() const
{
    std::string ret = "";
    
    if (getRoom() == nullptr) return ret;
    ret += getRoom()->getName() + "\n";
    ret += getRoom()->getDescription();
    return ret;
}

std::string Player::look(const std::string &actorOrItem) const
{
    std::string ret = "";

    if (getRoom() == nullptr) return ret;
    if (actorOrItem == "self" || actorOrItem == "myself")
    {
        return getDescription();
    }
    
    Actor *actor = getRoom()->getActor(actorOrItem);
    Item *item = getRoom()->getItem(actorOrItem);
    Item *inventory_item = this->getItem(actorOrItem);
    if (actor != nullptr)
        return actor->getDescription();
    else if (item != nullptr)
        return item->getDescription();
    else if (inventory_item != nullptr)
        return inventory_item->getDescription();
    else
        return "I don't see any " + actorOrItem + ".";
}

void Player::attackResponse(Actor *actor)
{
    //std::cout << "Player attack response" << std::endl;
    
    std::cout << "HP: " << hp << "/" << getStats().maxhp << std::endl;
}

Stats Player::addStats(const Stats &s2)
{
    stats += s2;
    return stats;
}

int Player::numberOfKills()
{
    return kills;
}

void Player::setKills(int k)
{
    kills = k;
}

void Player::addMoney(int k)
{
    money += k;
}

unsigned Player::getMoney()
{
    return money;
}

std::string Player::serialize(std::ostream &os) const
{
    std::string playerSym = gensym();
    os << playerSym << ":MAKE-PLAYER ";
    actorTypeIndependentSerializeConstructorParameters(os);
    os << " " << money << std::endl;
    actorTypeIndependentSerialize(os, playerSym);
    os << ":SET-NUMBER-KILLS " << playerSym << " " << kills << std::endl;
    return playerSym;
}
