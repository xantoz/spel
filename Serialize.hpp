#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <string>
#include <ostream>

#include "Room.hpp"

std::string gensym();
void serialize(const std::list<Room*> &rooms, std::ostream &os);



#endif /* SERIALIZE_HPP */
