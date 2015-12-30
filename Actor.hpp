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
    virtual void attackResponse(Actor* actor);
    
public:
    Actor(const std::string &name, const std::string &description);
    Actor(const std::string &name, const std::string &description, const Stats &stats);
    virtual ~Actor() override;
    
    
    Stats getTotalStats() const;
    void attack(Actor *actor);
    bool attack(const std::string&);
    void beAttacked(Actor* actor, unsigned int dmg);
    
    bool equipArmor(const std::string &name);
    bool equipShield(const std::string &shield);
    bool equipSword(const std::string &sword);
    bool equipShoes(const std::string &shoes);
    void unequipArmor();
    void unequipShield();
    void unequipSword();
    void unequipShoes();
    const Armor *getArmor() const;
    const Shield *getShield() const;
    const Sword *getSword() const;
    const Shoes *getShoes() const;
    
    void go(std::string direction);
    Room *getRoom() const;
    std::string use(const std::string &itemName);
    bool pickup(const std::string &itemName); // returns true if manage to pick up
    bool drop(const std::string &itemName); // returns true if manage to drop (can fail when naming non-existent item)
    
    const Stats &getStats() const;

    virtual std::string getDescription() const override;

    friend class Room;
    friend class Item; // KLUDGE: Needed so that Item can set the Armor etc. to nullptr in Item::~Item
};

#endif
