#include "Shoes.hpp"
#include "Serialize.hpp"

#include <iostream>

Shoes::Shoes(const std::string &name,
             const std::string &description,
             unsigned weight,
             const Stats &stats)
    : Equippable(name, description, weight, stats)
{
}


Shoes::~Shoes()
{
    std::cerr << "Shoes<" << this->getName() << "> destructor" << std::endl;
}

std::string Shoes::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-SHOES ";
    serializeEquippableCommonConstructorParameters(os);
    return sym;
}
