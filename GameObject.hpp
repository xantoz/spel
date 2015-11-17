#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
class GameObject 
{
private:
    std::string description;
    std::string name;
    
protected:
    GameObject();
public:
    virtual std::string getDescription();
    std::string getName();
    
}
    

#endif
