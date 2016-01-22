#include "Callback.hpp"

#include "Serialize.hpp"
#include <fstream>
#include <iostream>


Callback::Callback(const std::string &path) :
    callback_path(path)
{
}

const std::string &Callback::getCallback() const
{
    return callback_path;
}

void Callback::setCallback(const std::string &path)
{
    // std::cerr << "Setting callback to " << path << std::endl;
    callback_path = path;
}

void Callback::runCallback(std::initializer_list<std::pair<const std::string, GameObject*>> predef_vars)
{
    // std::cerr << "Running callback " << callback_path << std::endl;
    std::ifstream infile(callback_path);
    load(infile, predef_vars);
}
