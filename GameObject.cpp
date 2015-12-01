#include "GameObject.hpp"

GameObject::GameObject()
{
}

GameObject::GameObject(const std::string &_name, const std::string &_description) : name(_name), description(_description)
{
}

const std::string &GameObject::getBaseDescription() const
{
    return description;
}

std::string GameObject::getDescription() const
{
    return description;
}

const std::string &GameObject::getName() const
{
    return name;
}

GameObject::~GameObject()
{
}
