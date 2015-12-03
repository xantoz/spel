#include "Shop.hpp"
#include <algorithm>
#include "exceptions.hpp"
Shop::Shop(std::string &name, std::string &description, std::list<Item*> itms) : Room(name, description,"exit",  Room* exit, nullptr), inventory(itms) 
{
}
std::string Shop::listItems() const 
{
    std::ostringstream os;
    
    for(std::pair<Item, unsigned> itemPrice : inventory)
    {
        os << itemPrice.first.getName() << " " << itemPrice.second  << "\n";
    }
    return os.str();
}

const std::pair<Item, unsigned>& Shop:: getShopItem(const std::string &name)
{
    std::list<std::pair<Item, unsigned>> it =
        std::find_if(inventory.begin(), inventory.end(),
                     [&](const std::pair<Item,unsigned> &pair) {
                         return pair.first == name;
                     });
    
    if (it == inventory.end())
        throw NoSuchItemException();
    else
        return *it;
        
}

std::string Shop::getDescription() const 
{
    return Room::getDescription() + " " + listItems();
    
}
