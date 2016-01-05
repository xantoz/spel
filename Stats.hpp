#ifndef STATS_HPP
#define STATS_HPP

#include <string>

struct Stats
{
    int maxhp, str, atk, def, spd, acc, eva;
    Stats operator+(const Stats &s2) const;
    Stats operator-(const Stats &s2) const;
    Stats operator*(int scalar) const;
    Stats operator*(double scalar) const;
    Stats operator/(int scalar) const;
    Stats &operator+=(const Stats &s2);
    Stats &operator-=(const Stats &s2);
    Stats &operator+=(int scalar);
    Stats &operator-=(int scalar);

    std::string toString() const;
    std::string serializeString() const;
};

#endif    
