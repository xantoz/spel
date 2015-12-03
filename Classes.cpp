#include "Classes.hpp"
#include <cstdlib>
#include <array>

static Stats getRandomStats(int tmp2)
{
    int tmp1 = 0;
    std::array<int,7> st = {tmp1 = (std::random() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::random() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::random() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::random() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::random() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::random() % (tmp2 = (tmp2 - tmp1))),
                            tmp1 = (std::random() % (tmp2 = (tmp2 - tmp1)))};
    std::shuffle(st.begin(), st.end());        
    Stats random_stats = {st[0], st[1], st[2], st[3], st[4], st[5], st[6]};
    return random_stats;
    
}

Troll::Troll(const std::string &name, const std::string &description, int lvl) :
    Actor(name, description) 
{
    static const Stats base_stats = {10, 1, 2, 1, 1, 1, 1};
    
    stats = base_stats*lvl + getRandomStats(10);
}

    
Dragon::Dragon(const std::string &name, const std::string &description, int lvl) :
    Actor(name, description)
{
    static const Stats base_stats = {20, 2, 2, 4, 1, 1, 1};
    
    stats = base_stats*lvl+getRandomStats(10);
}

Thief::Thief(const std::string &name, const std::string &description, int lvl) :
    Actor(name, description)
{
    static const Stats base_stats = {10, 1, 2, 6, 1, 1, 1};
    
    stats = base_stats*lvl+getRandomStats(10);
}

Golem:: Golem(const std::string &name, const std::string &description, int lvl) :
    Actor(name, description)
{
    static const Stats base_stats = {10, 1, 10, 1, 1, 1, 1};
    
    stats = base_stats*lvl + getRandomStats(10);
}

Human::Human(const std::string &name, const std::string &description, const std::string &text) :
    Actor(name, description)
{
    static const Stats base_stats = {20, 1, 2, 1, 1, 1, 1};
    
    stats = base_stats*lvl+getRandomStats(10);
}
