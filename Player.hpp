#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Actor.hpp"
#include "Stats.hpp"
#include "Item.hpp"

#include <string>
#include <ostream>

class Player : public Actor
{
private:
    const static unsigned magisk_konstant = 10;
protected:
    virtual bool can_carry(const Item *i) const override;
    virtual void attackResponse(Actor *actor) override;
public:
    // Player(const std::string &name, const std::string &description);
    Player(const std::string &name, const std::string &description, const Stats &stats);
    ~Player();

    std::string look() const;
    std::string look(const std::string &actorOrItem) const;
    Stats addStats(const Stats &stats);

    virtual std::string serialize(std::ostream &os) const override;
};

#endif
