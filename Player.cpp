#include "Player.hpp"
#include "Room.hpp"
#include "Serialize.hpp"
#include <iostream>

// Player::Player(const std::string &name, const std::string &description) :
//     Actor(name, description)
// {
// }

Player::Player(const std::string &name, const std::string &description, const Stats &stats) :
    Actor(name, description, stats), money(0)
{
}

Player::Player(const std::string &name, const std::string &description, const Stats &stats, int hp) :
    Actor(name, description, stats, hp), money(0)
{
}

Player::~Player()
{
    std::cerr << "Player<" << getName() << "> destructor" << std::endl;
}

bool Player::can_carry(const Item *i) const
{
	unsigned total_weight = 0;
    for(Item *i: getItems())
    	total_weight += i->getWeight();
    
    return (total_weight < stats.str*magisk_konstant);
}

std::string Player::getDescription() const 
{
    std::string firstPart = Actor::getDescription();
    return firstPart + "\nMoney: " + std::to_string(money);
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
    if (actorOrItem == "self")
    {
        return getDescription();
    }
    
    Actor *a = getRoom()->getActor(actorOrItem);
    Item *i = getRoom()->getItem(actorOrItem);
    if (a != nullptr)
    {
        return a->getDescription();
    }
    else if (i != nullptr)
    {
        return i->getDescription();
    }
    else
    {
        return "I don't see any " + actorOrItem + ".";
    }
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

bool Player::hasKilledMonster()
{
    return killedMonster;
}

void Player::setKills(int k)
{
    kills = k;
}

void Player::setKilledMonster(bool k)
{
    killedMonster = k;
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
    actorTypeIndependentSerialize(os, playerSym);
    os << ":SET-KILLED-MONSTER " << playerSym << " " << killedMonster << std::endl;
    os << ":SET-NUMBER-KILLS " << playerSym << " " << kills << std::endl;
    return playerSym;
}
