#include "Potion.hpp"
#include <iostream>
Potion::Potion(const std::string &name, const std::string &desc, unsigned int weight, int lvl) :
    Item(name, desc, weight), hpHeals(10*lvl)
{
    consumable = true;
}

Potion::~Potion()
{
    std::cerr << "Potion<" << getName() << "> destructor" << std::endl;
}

std::string Potion::use(Actor* actor)
{
    int maxHP = actor->getStats().maxhp;
    if(actor->getHP() + hpHeals > maxHP)
    {
        actor->setHP(maxHP);
    }
    else 
    {
        actor->setHP(actor->getHP() + hpHeals);
    }
    used = true;
    return actor->getName() + " recovered hp!";
}

