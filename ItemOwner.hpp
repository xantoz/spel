#ifndef ITEMOWNER_HPP
#define ITEMOWNER_HPP

#include "Item.hpp"
#include "GameObject.hpp"

#include <list>
#include <string>
#include <ostream>

class ItemOwner : public GameObject
{
protected:
    std::list<Item*> items;

    virtual bool can_carry(const Item* item) const;

    ItemOwner(); // ItemOwners shouldn't be instantiatable
    ItemOwner(const std::string &name, const std::string &description);

public:
    /// Returns false if unable to add item
    bool addItem(Item *i);
    /// Will add the item even if the ItemOwner cannot carry more
    void addItemNoFail(Item *i); 
    /// throws NoSuchItemException if Item doesn't exist
    void removeItem(Item* i);

    /// Returns nullptr when item doesn't exist
    Item *getItem(const std::string &name) const;
    const std::list<Item*> &getItems() const;

    void serializeItems(std::ostream &os, const std::string &ownerSym) const;
    
    virtual ~ItemOwner();
};

#endif
