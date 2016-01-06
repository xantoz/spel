#ifndef ITEM_HPP
#define ITEM_HPP

#include "GameObject.hpp"
#include <string>
#include <ostream>

class ItemOwner;                                            // solve circular reference
class Actor;                                                // avoid circular deps
class Item : public GameObject
{
private:
    unsigned weight;
    ItemOwner *owner;
protected:
    bool used;
    bool consumable;
    
public:

    Item(const std::string &name, const std::string &description, unsigned weight);
    virtual ~Item() override;
    
    unsigned getWeight() const;
    ItemOwner *getOwner() const;
    bool usedUp() const;
    
    virtual std::string use(Actor* actor);

    virtual std::string serialize(std::ostream &os) const;
    
    friend class ItemOwner;
};

#endif
