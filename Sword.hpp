#ifndef SWORD_HPP
#define SWORD_HPP
#include "Equippable.hpp"
class Sword : public Equippable 
{
public:
    Sword(const std::string &name, const std::string &description, unsigned weight,  const Stats &stats);

    virtual ~Sword() override;
};
#endif
