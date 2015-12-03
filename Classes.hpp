#ifndef CLASSES_HPP
#define CLASSES_HPP
class Troll : Actor 
{
};

class Dragon : Actor
{

};

class Thief : Actor
{
};


class Human : Actor
{
public:
    Human(const std::string &name, const std::string &description, const std::string &text);
    
private:
    std::string text;
    
};


    

#endif
