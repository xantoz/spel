#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <string>

class GameObject 
{
private:
    std::string name;
    std::string description;
    
protected:
    GameObject();
    GameObject(const std::string &name, const std::string &description);
public:
    virtual std::string getDescription() const;
    const std::string &getName() const;
    
};
    

#endif
