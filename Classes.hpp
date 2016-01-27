#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <string>
#include "Actor.hpp"
#include "Callback.hpp"

class Troll : public Actor 
{
public:
    Troll(const std::string &name, int lvl);
    Troll(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Troll() override;
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};

class Dragon : public Actor
{
public:
    Dragon(const std::string &name, int lvl);
    Dragon(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Dragon() override;
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};

class Thief : public Actor
{
public:
    Thief(const std::string &name, int lvl);
    Thief(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Thief() override;
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};

class Golem : public Actor
{
public:
    Golem(const std::string &name, int lvl);
    Golem(const std::string &name, const std::string &description, const Stats &stats, int hp);
    virtual ~Golem() override;
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
};


class Human : public Actor
{
public:
    Human(const std::string &name, const std::string &desc, const std::string &text);
    Human(const std::string &name, const std::string &desc, const Stats &stats, const std::string &text);
    Human(const std::string &name, const std::string &desc, const Stats &stats, int hp, const std::string &text);
    virtual ~Human() override;
    
    virtual void update() override;
    virtual std::string serialize(std::ostream &os) const override;
    
    virtual void talk();
private:
    std::string text;
};

class CallbackHuman : public Human, public Callback
{
public:
    CallbackHuman(const std::string &name, const std::string &desc, const std::string &callback_path);
    CallbackHuman(const std::string &name, const std::string &desc, const Stats &stats, const std::string &callback_path);
    CallbackHuman(const std::string &name, const std::string &desc, const Stats &stats, int hp, const std::string &callback_path);
    virtual ~CallbackHuman() override;
    virtual void talk() override;
    virtual std::string serialize(std::ostream &os) const override;
};
    
    

#endif
