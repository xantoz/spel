#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <string>
#include <ostream>
#include <istream>
#include <list>
#include <initializer_list>
#include <utility>

class GameObject;
class Room;

void load(std::istream &is,
          std::initializer_list<std::pair<const std::string, GameObject*>> predef_vars={});

std::string gensym();
std::string stringify(const std::string &string);
std::string escapeString(const std::string &string);
void serialize(const std::list<Room*> &rooms, std::ostream &os);

#endif /* SERIALIZE_HPP */
