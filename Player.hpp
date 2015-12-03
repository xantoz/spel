#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Actor.hpp"
#include "Item.hpp"
#include <string>

class Player : public Actor
{
private:
    const static unsigned magisk_konstant = 10;
protected:
    virtual bool can_carry(const Item *i) const override;
public:
    Player(const std::string &name, const std::string &description);
    ~Player();

    std::string look() const;
    std::string look(const std::string &actorOrItem) const;
};

#endif
