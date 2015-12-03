#ifndef ITEMOWNER_HPP
#define ITEMOWNER_HPP

#include "Item.hpp"
#include "GameObject.hpp"
#include <list>

class ItemOwner : public GameObject
{
private:
    std::list<Item*> items;
    
protected:
    virtual bool can_carry(const Item* item) const;

    ItemOwner(); // ItemOwners shouldn't be instantiatable
    ItemOwner(const std::string &name, const std::string &description);

public:
    /// Returns false if unable to add item
    bool addItem(Item *i);
    /// throws NoSuchItemException if Item doesn't exist
    void removeItem(Item* i);
    /// Returns nullptr when item doesn't exist
    Item *getItem(const std::string &name);
    const std::list<Item*> &getItems() const;
        
    virtual ~ItemOwner();
};

#endif
