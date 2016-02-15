#include "Equippable.hpp"
#include "Serialize.hpp"

#include <iostream>

Equippable::Equippable(const std::string &name, const std::string &description, unsigned weight, const Stats &s) :
    Item(name, description, weight), stats(s)
{
}

Equippable::~Equippable()
{
    std::cerr << "Equippable<" << this->getName() << "> destructor" << std::endl;
}


const Stats &Equippable::getStats() const
{
    return stats;
}

void Equippable::use(Actor* actor)
{
    std::cout
        << "You have to equip this item using the \"equip\" command."
        << std::endl;
    return;
}

std::string Equippable::getDescription() const
{
    return Item::getDescription() + "\nSTATS: " + stats.toString();
}

void Equippable::serializeEquippableCommonConstructorParameters(std::ostream &os) const
{
    os << stringify(getName()) << " " << stringify(getBaseDescription()) << " " << getWeight() << " " << stats.serializeString() << std::endl;
}

std::string Equippable::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-EQUIPPABLE ";
    serializeEquippableCommonConstructorParameters(os);
    return sym;
}

