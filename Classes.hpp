#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <string>
#include "Actor.hpp"

class Troll : public Actor 
{
public:
    Troll(const std::string &name, const std::string &description, int lvl);
    
};

class Dragon : public Actor
{
public:
    Dragon(const std::string &name, const std::string &description, int lvl);
    
};

class Thief : public Actor
{
public:
    Thief(const std::string &name, const std::string &description, int lvl);
    
};

class Golem : public Actor
{
public:
    Golem(const std::string &name, const std::string &description, int lvl);
};


class Human : public Actor
{
public:
    Human(const std::string &name, const std::string &description, const std::string &text);
    
private:
    std::string text;
    
};


    

#endif
