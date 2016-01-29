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
    void setDescription(const std::string&);
    void setDescription(std::string&&);
    void appendDescription(const std::string&);
    const std::string &getName() const;

    virtual ~GameObject();
};

std::ostream& operator<<(std::ostream &os, const GameObject& g);

#endif
