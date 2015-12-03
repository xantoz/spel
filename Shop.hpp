#ifndef SHOP_HPP
#define SHOP_HPP
class Shop : public Room 
{
private:
    std::list<std::pair<Item, unsigned>> inventory;

public:
    Shop(std::string &name, std::string &description, std::list<Item> &itms);
    std::string listInventory() const;
    const std::pair<Item, unsigned>& getShopItem(const std::string &name) const;
    virtual std::string getDescription() const override;
    
};

    
