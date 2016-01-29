#include "GameObject.hpp"
#include <iostream>

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

void GameObject::setDescription(const std::string &newdesc)
{
    description = newdesc;
}

void GameObject::setDescription(std::string &&newdesc)
{
    description = newdesc;
}

void GameObject::appendDescription(const std::string &appdesc)
{
    description = description + "\n" + appdesc;
}

const std::string &GameObject::getName() const
{
    return name;
}

GameObject::~GameObject()
{
    std::cerr << "GameObject<" << name << "> destructor" << std::endl;
}

std::ostream& operator<<(std::ostream &os, const GameObject& g)
{
    os << g.getName() << " " << g.getDescription();
    return os;
}
