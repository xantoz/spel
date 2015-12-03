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
    // non-virtual method meant for simply accessing what's in the string description
    const std::string &getBaseDescription() const;
    virtual std::string getDescription() const;
    const std::string &getName() const;

    virtual ~GameObject();
};
    

#endif
