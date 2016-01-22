#ifndef ITEM_HPP
#define ITEM_HPP

#include "GameObject.hpp"
#include "Callback.hpp"
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
    
    virtual void use(Actor* actor);

    virtual std::string serialize(std::ostream &os) const;
    
    friend class ItemOwner;
};

class CallbackItem : public Item, public Callback
{
public:
    CallbackItem(const std::string &name,
                 const std::string &description,
                 unsigned weight);
    CallbackItem(const std::string &name,
                 const std::string &description,
                 unsigned weight,
                 const std::string &callback_path);
    virtual ~CallbackItem();
    virtual void use(Actor* actor) override;
    virtual std::string serialize(std::ostream &os) const override;

    void setConsumable(bool);
    void setUsed(bool);
};


#endif
