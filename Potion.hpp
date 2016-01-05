#ifndef POTION_HPP
#define POTION_HPP

#include "Item.hpp"
#include "Actor.hpp"

class Potion : public Item
{
private:
    int hpHeals;
public:
    Potion(const std::string &name, const std::string &desc, unsigned int weight, int lvl);
    virtual ~Potion() override;
    virtual std::string use(Actor* actor) override;
};

#endif    
