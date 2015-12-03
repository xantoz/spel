#ifndef STATS_HPP
#define STATS_HPP

struct Stats
{
    int maxhp, str, atk, def, spd, acc, eva;
    Stats operator+(const Stats &s2) const;
    Stats operator-(const Stats &s2) const;
    Stats operator*(int scalar) const;
    Stats operator/(int scalar) const;
    Stats &operator+=(const Stats &s2);
    Stats &operator-=(const Stats &s2);
    Stats &operator+=(int scalar);
    Stats &operator-=(int scalar);
};

#endif    
