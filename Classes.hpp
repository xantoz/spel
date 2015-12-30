#ifndef CLASSES_HPP
#define CLASSES_HPP
class Troll : Actor 
{
public:
    Troll(const std::string &name, const std::string &description, int lvl);
    
};

class Dragon : Actor
{
public:
    Dragon(const std::string &name, const std::string &description, int lvl);
    
};

class Thief : Actor
{
public:
    Thief(const std::string &name, const std::string &description, int lvl);
    
};

class Golem : Actor
{
public:
    Golem(const std::string &name, const std::string &description, int lvl);
};


class Human : Actor
{
public:
    Human(const std::string &name, const std::string &description, const std::string &text);
    
private:
    std::string text;
    
};


    

#endif
