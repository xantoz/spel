#ifndef SHOP_HPP
#define SHOP_HPP
#include "Item.hpp"
#include "Room.hpp"
#include <list>
class Shop : public Room 
{
private:
    std::list<std::pair<Item*, unsigned>> inventory;

public:
    Shop(const std::string &name, const std::string &description, std::list<std::pair<Item*, unsigned>> &itms);
    std::string listInventory() const;
    bool removeShopItem(std::pair<Item*, unsigned> item);
    const std::pair<Item*, unsigned>& getShopItem(const std::string &name) const;
    virtual std::string getDescription() const override;
    virtual std::string serialize(std::ostream &os) const override;
};
#endif

    
