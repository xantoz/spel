#include "Classes.hpp"
#include "Serialize.hpp"
#include "Room.hpp"
#include "Shop.hpp"
#include "main.hpp"
#include "CashCard.hpp"
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

static void updateActor(Actor* actor)
{
    for(auto &exit : actor->getRoom()->getExits())
    {   
        if (std::rand() % 100 < 5)
        {
            if (dynamic_cast<Shop*>(exit.second) == nullptr) // don't enter shops
            {
                // std::cerr << actor->getName() << " moved from " << actor->getRoom()->getName() << " to " << exit.first << std::endl;

                if (actor->getRoom() == player->getRoom())
                    std::cout << actor->getName() << " went to the " << exit.first << std::endl;
                if (exit.second == player->getRoom())
                    std::cout << actor->getName() << " walked into " << player->getRoom()->getName() << std::endl;
                
                exit.second->addActor(actor);
            }
            
            break;
        }
    }
}

static const Stats troll_base_stats = {10, 1, 3, 5, 4, 4, 3};
static const Stats dragon_base_stats = {20, 3, 3, 6, 4, 5, 3};
static const Stats thief_base_stats = {10, 2, 3, 2, 8, 4, 7};
static const Stats golem_base_stats = {10, 6, 4, 12, 2, 4, 3};
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
    if (std::rand() % 2 == 0)
    {
        CashCard* c = new CashCard(lvl);
        addItem(c);
    }
}

Troll::Troll(const std::string &name, const std::string &description, const Stats &stats, int hp) :
    Actor(name, description, stats, hp)
{
}


Troll::~Troll()
{
    std::cerr << "Troll<" << this->getName() << "> destructor" << std::endl;
}

void Troll::update()
{
    if (!noWander)
        updateActor(this);
}

std::string Troll::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-TROLL ";
    actorTypeIndependentSerializeConstructorParameters(os);
    os << std::endl;
    actorTypeIndependentSerialize(os, sym);
    return sym;
}
    
Dragon::Dragon(const std::string &name, int lvl) :
    Actor(name, classAndLevel("Dragon", lvl), dragon_base_stats*lvl + getRandomStats(10))
{
    if (std::rand() % 2 == 0)
    {
        CashCard* c = new CashCard(4*lvl);
        addItem(c);
    }
}

Dragon::Dragon(const std::string &name, const std::string &description, const Stats &stats, int hp) :
    Actor(name, description, stats, hp)
{
}

Dragon::~Dragon()
{
    std::cerr << "Dragon<" << this->getName() << "> destructor" << std::endl;
}

void Dragon::update()
{
    if (!noWander)
        updateActor(this);
}


std::string Dragon::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-DRAGON ";
    actorTypeIndependentSerializeConstructorParameters(os);
    os << std::endl;
    actorTypeIndependentSerialize(os, sym);
    return sym;
}

Thief::Thief(const std::string &name,  int lvl) :
    Actor(name, classAndLevel("Thief", lvl), thief_base_stats*lvl + getRandomStats(10))
{
    if (std::rand() % 2 == 0)
    {
        CashCard* c = new CashCard(lvl*2);
        addItem(c);
    }
}

Thief::Thief(const std::string &name, const std::string &description, const Stats &stats, int hp) :
    Actor(name, description, stats, hp)
{
}

Thief::~Thief()
{
    std::cerr << "Thief<" << this->getName() << "> destructor" << std::endl;
}

void Thief::update()
{
    if (!noWander)
        updateActor(this);
}

std::string Thief::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-THIEF ";
    actorTypeIndependentSerializeConstructorParameters(os);
    os << std::endl;
    actorTypeIndependentSerialize(os, sym);
    return sym;
}

Golem::Golem(const std::string &name,  int lvl) :
    Actor(name, classAndLevel("Golem", lvl), golem_base_stats*lvl + getRandomStats(10))
{
    if (std::rand() % 2 == 0)
    {
        CashCard* c = new CashCard(lvl*3);
        addItem(c);
    }
}

Golem::Golem(const std::string &name, const std::string &description, const Stats &stats, int hp) :
    Actor(name, description, stats, hp)
{
}


Golem::~Golem()
{
    std::cerr << "Golem<" << this->getName() << "> destructor" << std::endl;
}

void Golem::update()
{
    if (!noWander)
        updateActor(this);
}

std::string Golem::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-GOLEM ";
    actorTypeIndependentSerializeConstructorParameters(os);
    os << std::endl;
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

Human::Human(const std::string &name, const std::string &desc, const Stats &stats, int hp, const std::string &t) :
    Actor(name, desc, stats, hp), text(t)
{
}

Human::~Human()
{
    std::cerr << "Human<" << this->getName() << "> destructor" << std::endl;
}

void Human::update()
{
}

void Human::talk()
{
    std::cout << text << std::endl;
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

CallbackHuman::CallbackHuman(const std::string &name, const std::string &desc, const std::string &callback_path) :
    Human(name, desc, "unused"), Callback(callback_path)
{
}

CallbackHuman::CallbackHuman(const std::string &name, const std::string &desc, const Stats &stats, const std::string &callback_path) :
    Human(name, desc, stats, "unused"), Callback(callback_path)
{
}

CallbackHuman::CallbackHuman(const std::string &name, const std::string &desc, const Stats &stats, int hp, const std::string &callback_path) :
    Human(name, desc, stats, hp, "unused"), Callback(callback_path)
{
}

CallbackHuman::~CallbackHuman()
{
    std::cerr << "CallbackHuman<" << this->getName() << "> destructor" << std::endl;
}

void CallbackHuman::talk()
{
    runCallback({{"THIS", this}});
}

std::string CallbackHuman::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-CALLBACK-HUMAN "
       << stringify(getName()) << " "
       << stringify(getBaseDescription()) << " "
       << stats.serializeString() << " "
       << hp << " "
       << stringify(getCallback()) << std::endl;
    actorTypeIndependentSerialize(os, sym);
    return sym;
}


//////////////////
//// LastBoss ////
//////////////////
LastBoss::LastBoss(const std::string &name,
                   const std::string &desc,
                   const std::string &callback_path) :
    CallbackHuman(name, desc, callback_path)
{
}

LastBoss::LastBoss(const std::string &name,
                   const std::string &desc,
                   const Stats &stats,
                   const std::string &callback_path) :
    CallbackHuman(name, desc, stats, callback_path)
{
}

LastBoss::LastBoss(const std::string &name,
                   const std::string &desc,
                   const Stats &stats,
                   int hp,
                   const std::string &callback_path) :
    CallbackHuman(name, desc, stats, hp, callback_path)
{
}

LastBoss::~LastBoss()
{
    std::cerr << "LastBoss<" << this->getName() << "> destructor" << std::endl;
}

void LastBoss::die()
{
    CallbackHuman::die();
    std::cout << "You have defeated the Last Boss, " << getName() << "." << std::endl;
    
    win();                                                  // You won the game.
}

std::string LastBoss::serialize(std::ostream &os) const
{
    std::string sym = gensym();
    os << sym << ":MAKE-LAST-BOSS "
       << stringify(getName()) << " "
       << stringify(getBaseDescription()) << " "
       << stats.serializeString() << " "
       << hp << " "
       << stringify(getCallback()) << std::endl;
    actorTypeIndependentSerialize(os, sym);
    return sym;
}
