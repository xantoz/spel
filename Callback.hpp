#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include <string>
#include <initializer_list>
#include <utility>

class GameObject;

class Callback 
{
private:
    std::string callback_path;
public:
    Callback(const std::string &path);
    const std::string &getCallback() const;
    void setCallback(const std::string &path);
protected:
    void runCallback(std::initializer_list<std::pair<const std::string, GameObject*>> predef_vars={});
};
    

#endif
