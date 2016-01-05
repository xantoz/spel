#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <string>
#include "Actor.hpp"

class Troll : public Actor 
{
public:
    Troll(const std::string &name, int lvl);
    
};

class Dragon : public Actor
{
public:
    Dragon(const std::string &name, int lvl);
    
};

class Thief : public Actor
{
public:
    Thief(const std::string &name,  int lvl);
    
};

class Golem : public Actor
{
public:
    Golem(const std::string &name,  int lvl);
};


class Human : public Actor
{
public:
    Human(const std::string &name, const std::string &desc, const std::string &text);
    Human(const std::string &name, const std::string &desc, const std::string &text, Stats &stats);
    
    bool talk();
private:
    std::string text;
    
};


    

#endif
