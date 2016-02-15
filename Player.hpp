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
    unsigned int kills;
    unsigned int money;

    unsigned max_carry() const;
    
protected:
    virtual bool can_carry(const Item *i) const override;
public:
    // Player(const std::string &name, const std::string &description);
    Player(const std::string &name, const std::string &description, const Stats &stats);
    Player(const std::string &name, const std::string &description, const Stats &stats, int hp);
    Player(const std::string &name, const std::string &description, const Stats &stats, int hp, int money);
    virtual ~Player() override;
    virtual void attackResponse(Actor *actor) override;
    virtual std::string getDescription() const override;
    
    std::string look() const;
    std::string look(const std::string &actorOrItem) const;
    Stats addStats(const Stats &stats);
    int numberOfKills();
    void setKills(int k);
    void addMoney(int k);
    unsigned getMoney();
    
    virtual std::string serialize(std::ostream &os) const override;
};

#endif
