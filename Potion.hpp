#ifndef POTION_HPP
#define POTION_HPP

#include "Item.hpp"
#include "Actor.hpp"

#include <ostream>

class Potion : public Item
{
private:
    int hpHeals;
    int level;
    
public:
    // Potion(const std::string &name, const std::string &desc, unsigned int weight, int lvl);
    Potion(const std::string &name, int lvl);
    
    virtual ~Potion() override;
    virtual void use(Actor* actor) override;
    
    virtual std::string serialize(std::ostream&) const override;
};

#endif    
