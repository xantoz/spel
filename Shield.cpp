#include "Shield.hpp"
#include "Serialize.hpp"

#include <iostream>

Shield::Shield(const std::string &name,
               const std::string &description,
               unsigned weight,
               const Stats &stats) :
    Equippable(name, description, weight, stats)
{
}

Shield::~Shield()
{
    std::cerr << "Shield<" << this->getName() << "> destructor" << std::endl;
}

std::string Shield::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-SHIELD ";
    serializeEquippableCommonConstructorParameters(os);
    return sym;
}
