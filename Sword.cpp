#include "Sword.hpp"
#include "Serialize.hpp"

#include <iostream>

Sword::Sword(const std::string &name,
             const std::string &description,
             unsigned weight,
             const Stats &stats) :
    Equippable(name, description, weight, stats)
{
}

Sword::~Sword()
{
    std::cerr << "Sword<" << this->getName() << "> destructor" << std::endl;
}


std::string Sword::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-SWORD ";
    serializeEquippableCommonConstructorParameters(os);
    return sym;
}

