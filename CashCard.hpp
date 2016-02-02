#ifndef CASHCARD_HPP
#define CASHCARD_HPP
#include "Item.hpp"
class CashCard : public Item 
{

private:
    int level;
public:
    CashCard(int lvl);
    CashCard(const std::string &name, int lvl);
    
    virtual ~CashCard() override;
    virtual void use(Actor* actor) override;
    
    virtual std::string serialize(std::ostream&) const override;
};
    
#endif
