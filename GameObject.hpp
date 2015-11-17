#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <string>

class GameObject 
{
private:
    std::string description;
    std::string name;
    
protected:
    GameObject();
public:
    virtual std::string getDescription() const;
    const std::string &getName() const;
    
};
    

#endif
