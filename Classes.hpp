#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <string>
#include "Actor.hpp"

class Troll : public Actor 
{
public:
    Troll(const std::string &name, int lvl);
    Troll(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Troll();
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};

class Dragon : public Actor
{
public:
    Dragon(const std::string &name, int lvl);
    Dragon(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Dragon();
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};

class Thief : public Actor
{
public:
    Thief(const std::string &name, int lvl);
    Thief(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Thief();
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};

class Golem : public Actor
{
public:
    Golem(const std::string &name, int lvl);
    Golem(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Golem();
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};


class Human : public Actor
{
public:
    Human(const std::string &name, const std::string &desc, const std::string &text);
    Human(const std::string &name, const std::string &desc, const Stats &stats, const std::string &text);
    Human(const std::string &name, const std::string &desc, const Stats &stats, int hp, const std::string &text);
    virtual ~Human();
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
    
    void talk();
private:
    std::string text;
};


    

#endif
