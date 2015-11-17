#include "Item.hpp"

Item::Item(std::string nam, std::string descriptio, unsigned weigh); : owner(nullptr), name(nam), description(descriptio), weight(weigh)
{
}

const std::string &Item::getName() const
{
    return name;
}

const std::string &Item::getDescription() const
{
    return description;
}

unsigned Item::getWeight() const
{
    return weight;
}


