#include "GameObject.hpp"

GameObject::GameObject()
{
}

std::string GameObject::getDescription() const
{
    return description;
}

const std::string &GameObject::getName() const
{
    return name;
}

