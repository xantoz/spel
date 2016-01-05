#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <string>
#include "Actor.hpp"

class Troll : public Actor 
{
public:
    Troll(const std::string &name, int lvl);
    virtual ~Troll();

    virtual std::string serialize(std::ostream &os) const override;
};

class Dragon : public Actor
{
public:
    Dragon(const std::string &name, int lvl);
    virtual ~Dragon();

    virtual std::string serialize(std::ostream &os) const override;
};

class Thief : public Actor
{
public:
    Thief(const std::string &name, int lvl);
    virtual ~Thief();

    virtual std::string serialize(std::ostream &os) const override;
};

class Golem : public Actor
{
public:
    Golem(const std::string &name, int lvl);
    virtual ~Golem();

    virtual std::string serialize(std::ostream &os) const override;
};


class Human : public Actor
{
public:
    Human(const std::string &name, const std::string &desc, const std::string &text);
    Human(const std::string &name, const std::string &desc, const Stats &stats, const std::string &text);
    virtual ~Human();

    virtual std::string serialize(std::ostream &os) const override;
    
    bool talk();
private:
    std::string text;
};


    

#endif
