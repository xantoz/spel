#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "ItemOwner.hpp"
#include "Stats.hpp"
#include "Armor.hpp"
#include "Sword.hpp"
#include "Shoes.hpp"
#include "Shield.hpp"

#include <string>
#include <map>
#include <ostream>

class Room;                                                 // solve circular reference
class Actor : public ItemOwner 
{
private:
    Room *room;
    std::map<std::string, Room*> deathExits;
    bool dropItems; // if True we drop our items and equipment at death

protected:    
    Stats stats;

    int hp;
    bool dead;
    Armor *armor;
    Shield *shield;
    Sword *sword;
    Shoes *shoes;
    virtual void attackResponse(Actor *actor);
    // To be used be inheriting classes when serializing
    void actorTypeIndependentSerializeConstructorParameters(std::ostream &os) const;
    void actorTypeIndependentSerialize(std::ostream &os, const std::string &actorSym) const;

public:
    Actor(const std::string &name, const std::string &description);
    Actor(const std::string &name, const std::string &description, const Stats &stats);
    Actor(const std::string &name, const std::string &description, const Stats &stats, int hp);
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

    int getHp() const;
    void setHp(int hp);

    // If we set this to true we drop our items and equipment on death instead of deleting them
    void setDrop(bool drop);
    
    // When using this one must ensure that the Room passed lives longer than this Actor
    void setDeathExit(const std::string &name, Room *room);
    const std::map<std::string, Room*> &getDeathExits() const;
    
    void go(std::string direction);
    Room *getRoom() const;
    std::string use(const std::string &itemName);
    bool pickup(const std::string &itemName); // returns true if manage to pick up
    bool drop(const std::string &itemName); // returns true if manage to drop (can fail when naming non-existent item)
    
    const Stats &getStats() const;

    virtual std::string getDescription() const override;

    virtual std::string serialize(std::ostream &os) const;

    virtual void die();
    bool isDead();
    friend class Room;
};

#endif
