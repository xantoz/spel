#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Actor.hpp"
#include "Stats.hpp"
#include "Item.hpp"
#include <string>

class Player : public Actor
{
private:
    const static unsigned magisk_konstant = 10;
protected:
    virtual bool can_carry(const Item *i) const override;
    virtual attackResponse(Actor *actor);
public:
    // Player(const std::string &name, const std::string &description);
    Player(const std::string &name, const std::string &description, const Stats &stats);
    ~Player();

    std::string look() const;
    std::string look(const std::string &actorOrItem) const;
};

#endif
