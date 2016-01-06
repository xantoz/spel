#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <string>
#include <ostream>
#include <istream>
#include <list>

class Room;

void load(std::istream &is);

std::string gensym();
std::string stringify(const std::string &string);
std::string escapeString(const std::string &string);
void serialize(const std::list<Room*> &rooms, std::ostream &os);

#endif /* SERIALIZE_HPP */
