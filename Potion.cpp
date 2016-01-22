#include "Potion.hpp"

#include "Serialize.hpp"
#include <iostream>
#include <string>

// Potion::Potion(const std::string &name, const std::string &desc, unsigned int weight, int lvl) :
//     Item(name, desc, weight), hpHeals(10*lvl), level(lvl)
// {
//     consumable = true;
// }

Potion::Potion(const std::string &name, int lvl) :
    Item(name, "Potion that heals" + std::to_string(lvl*10) + "hp", 2*lvl), hpHeals(lvl*10), level(lvl)
{
    consumable = true;
}


Potion::~Potion()
{
    std::cerr << "Potion<" << getName() << "> destructor" << std::endl;
}

void Potion::use(Actor* actor)
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
    std::cout << actor->getName() + " recovered hp!" << std::endl;
    return;
}

std::string Potion::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-POTION " << stringify(getName()) << " " << level << std::endl;
    return sym;
}
