#include "Item.hpp"
#include "ItemOwner.hpp"

Item::Item(std::string name, std::string description, unsigned w) :
    GameObject(name, description),
    owner(nullptr),
    weight(w)
{
}

unsigned Item::getWeight() const
{
    return weight;
}
