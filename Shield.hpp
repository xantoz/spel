#ifndef SHIELD_HPP
#define SHIELD_HPP
#include "Equippable.hpp"

class Shield : public Equippable
{
public:
    Shield(const std::string &name, const std::string &description, unsigned weight, const Stats &stats);

    virtual ~Shield() override;
};
#endif
