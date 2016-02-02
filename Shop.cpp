#include "Shop.hpp"
#include "exceptions.hpp"
#include "Serialize.hpp"

#include <algorithm>
#include <sstream>

Shop::Shop(const std::string &name, const std::string &description, std::list<std::pair<Item*, unsigned>> &itms) : Room(name, description, nullptr), inventory(itms) 
{
}

Shop::~Shop()
{
    std::cerr << "Shop<" << getName() << "> destructor" << std::endl;
    for (auto item : inventory)
    {
        delete item.first;
    }
}

std::string Shop::listInventory() const 
{
    std::ostringstream os;
    
    for(std::pair<Item*, unsigned> itemPrice : inventory)
    {
os << itemPrice.first->getName() << " " << itemPrice.second << "\n";
    }
    return os.str();
}

bool Shop::removeShopItem(std::pair<Item*, unsigned> item)
{
    inventory.remove(item);
    return true;
}


const std::pair<Item*, unsigned>& Shop::getShopItem(const std::string &name) const
{
    auto it =
        std::find_if(inventory.begin(), inventory.end(),
                     [&](const std::pair<Item*,unsigned> &pair) {
                         return pair.first->getName() == name;
                     });
    
    if (it == inventory.end())
        throw NoSuchItemException();
    else
        return *it;
}

std::string Shop::getDescription() const 
{
    return Room::getDescription() + "\n\n" + listInventory();
}

std::string Shop::serialize(std::ostream &os) const
{
    std::string actorSym = gensym();
    std::vector<std::string> itemsyms;
    for (const std::pair<Item*, unsigned> &itemAndPrice : inventory)
    {
        itemsyms.emplace_back(itemAndPrice.first->serialize(os));
    }
    
    os << actorSym << ":MAKE-SHOP ";
    os << stringify(getName()) << " ";
    os << stringify(getBaseDescription()) << " ";
    int i = 0;
    for (const std::pair<Item*, unsigned> &itemAndPrice : inventory)
    {
        os << itemsyms[i] << " " << itemAndPrice.second << " ";
        ++i;
    }
    os << std::endl;
    
    return actorSym;
}

