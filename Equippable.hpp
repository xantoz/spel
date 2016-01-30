#ifndef EQUIPPABLE_HPP
#define EQUIPPABLE_HPP

#include "Item.hpp"
#include "Stats.hpp"

#include <string>
#include <ostream>

class Equippable : public Item
{
private:
    const Stats stats;
protected:
    void serializeEquippableCommonConstructorParameters(std::ostream &os) const;
    
public:
    Equippable(const std::string &name, const std::string &description, unsigned weight, const Stats &s);
    virtual ~Equippable() override;

    const Stats &getStats() const;

    virtual std::string getDescription() const override;
    virtual std::string serialize(std::ostream &os) const override;
};
#endif
