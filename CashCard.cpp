#include "CashCard.hpp"

#include "Player.hpp"
#include "Serialize.hpp"

#include <iostream>
#include <string>
#include <map>

static std::string GenCashCardName(int lvl)
{
    static int counter = 0;
    return "CashCard" + std::to_string(++counter);
}

CashCard::CashCard(int lvl) :
    CashCard(GenCashCardName(lvl), lvl)
{
}

CashCard::CashCard(const std::string &name, int lvl) :
    Item(name, "CashCard that has the value of  " + std::to_string(lvl*10) + " gold", 10), level(lvl)
{
    consumable = true;
}

CashCard::~CashCard()
{
    std::cerr << "CashCard<" << getName() << "> destructor" << std::endl;
}

void CashCard::use(Actor* actor)
{
    
    Player* p = dynamic_cast<Player*>(actor);
    if(p == nullptr)
    {
        std::cout << "Can only be used on your self!" << std::endl;
    }
    else 
    {
        std::cout << "You earned " << level*10 << " gold" << std::endl;
        p->addMoney(level*10);
    }
    used = true;
    return;
}

std::string CashCard::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-CASHCARD " << stringify(getName()) << " " << level << std::endl;
    return sym;
}
