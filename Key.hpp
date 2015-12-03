#ifndef KEY_HPP
#define KEY_HPP
#include "Item.hpp"
class Key : public Item 
{
public:
    Key(const std::string &name, const std::string &description, unsigned weight);
    
}
    
