#include "Classes.hpp"
#include "Serialize.hpp"

#include <cstdlib>
#include <array>
#include <algorithm>
#include <sstream>
#include <iostream>

static Stats getRandomStats(int tmp2)
{
    int tmp1 = 0;
    std::array<int,7> st = {tmp1 = (std::rand() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::rand() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::rand() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::rand() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::rand() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::rand() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::rand() % (tmp2 = (tmp2 - tmp1)))};
    std::random_shuffle(st.begin(), st.end());        
    Stats random_stats = {st[0], st[1], st[2], st[3], st[4], st[5], st[6]};
    return random_stats;
    
}
static const Stats troll_base_stats = {10, 1, 3, 4, 20, 2, 2};
static const Stats dragon_base_stats = {20, 3, 3, 6, 4, 5, 2};
static const Stats thief_base_stats = {10, 2, 3, 2, 8, 2, 7};
static const Stats golem_base_stats = {10, 6, 4, 12, 2, 2, 2};
static const Stats human_base_stats = {20, 1, 2, 1, 1, 1, 1};
static std::string classAndLevel(const std::string &c, int lvl)
{
    std::ostringstream ostring;
    ostring << c << " level "<< lvl;
    return ostring.str();
}

Troll::Troll(const std::string &name, int lvl) :
    Actor(name, classAndLevel("Troll", lvl), troll_base_stats*lvl + getRandomStats(10))
{
}

Troll::~Troll()
{
    std::cerr << "Troll<" << this->getName() << "> destructor" << std::endl;
}

std::string Troll::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-TROLL ";
    actorTypeIndependentSerializeConstructorParameters(os);
    actorTypeIndependentSerialize(os, sym);
    return sym;
}
    
Dragon::Dragon(const std::string &name, int lvl) :
    Actor(name, classAndLevel("Dragon", lvl), dragon_base_stats*lvl + getRandomStats(10))
{
}

Dragon::~Dragon()
{
    std::cerr << "Dragon<" << this->getName() << "> destructor" << std::endl;
}

std::string Dragon::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-DRAGON ";
    actorTypeIndependentSerializeConstructorParameters(os);
    actorTypeIndependentSerialize(os, sym);
    return sym;
}

Thief::Thief(const std::string &name,  int lvl) :
    Actor(name, classAndLevel("Thief", lvl), thief_base_stats*lvl + getRandomStats(10))
{
}

Thief::~Thief()
{
    std::cerr << "Thief<" << this->getName() << "> destructor" << std::endl;
}

std::string Thief::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-THIEF ";
    actorTypeIndependentSerializeConstructorParameters(os);
    actorTypeIndependentSerialize(os, sym);
    return sym;
}

Golem::Golem(const std::string &name,  int lvl) :
    Actor(name, classAndLevel("Golem", lvl), golem_base_stats*lvl + getRandomStats(10))
{
}

Golem::~Golem()
{
    std::cerr << "Golem<" << this->getName() << "> destructor" << std::endl;
}

std::string Golem::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-GOLEM ";
    actorTypeIndependentSerializeConstructorParameters(os);
    actorTypeIndependentSerialize(os, sym);
    return sym;
}


Human::Human(const std::string &name, const std::string &desc, const std::string &t) :
    Actor(name, desc, human_base_stats + getRandomStats(10)), text(t)
{
}

Human::Human(const std::string &name, const std::string &desc, const Stats &stats, const std::string &t) :
    Actor(name, desc, stats), text(t)
{
}

Human::~Human()
{
    std::cerr << "Human<" << this->getName() << "> destructor" << std::endl;
}
bool Human::talk()
{
    std::cout << text << std::endl;
    return false;
}

std::string Human::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-HUMAN "
       << stringify(getName()) << " "
       << stringify(getBaseDescription()) << " "
       << stats.serializeString() << " "
       << hp << " "
       << stringify(text) << std::endl;
    actorTypeIndependentSerialize(os, sym);
    return sym;
}

