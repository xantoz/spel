#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "ItemOwner.hpp"
#include "Stats.hpp"
#include "Armor.hpp"
#include "Sword.hpp"
#include "Shoes.hpp"
#include "Shield.hpp"
#include <string>

class Room;                                                 // solve circular reference
class Actor : public ItemOwner 
{
private:
    Room *room;

protected:    
    Stats stats;

    Armor *armor;
    Shield *shield;
    Sword *sword;
    Shoes *shoes;
    
public:
    Actor(const std::string &name, const std::string &description);
    Actor(const std::string &name, const std::string &description, const Stats &stats);
    virtual ~Actor() override;

    bool equipArmor(const std::string &name);
    bool equipShield(const std::string &shield);
    bool equipSword(const std::string &sword);
    bool equipShoes(const std::string &shoes);
    Armor *getArmor();
    Shield *getShield();
    Sword *getSword();
    Shoes *getShoes();

    void go(std::string direction);
    Room *getRoom() const;
    std::string use(const std::string &itemName);
    bool pickup(const std::string &itemName); // returns true if manage to pick up
    bool drop(const std::string &itemName); // returns true if manage to drop (can fail when naming non-existent item)
    
    const Stats &getStats() const;

    virtual std::string getDescription() const override;

    friend class Room;
};

#endif
