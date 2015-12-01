#ifndef STATS_HPP
#define STATS_HPP
struct Stats
{
    int maxhp, atk, def, spd, acc, eva;
    Stats operator+(const& Stats s2) const 
    {
        Stats s;
        s.maxhp = maxhp + s2.maxhp;
        s.atk = atk + s2.atk;
        s.def = def + s2.def;
        s.spd = sps + s2.spd;
        s.acc = acc + s2.acc;
        s.eva = eva + s2.eva;
        return s;
    }
    Stats operator-(const& Stats s2) const
    {
        Stats s;
        s.maxhp = maxhp - s2.maxhp;
        s.atk = atk - s2.atk;
        s.def = def - s2.def;
        s.spd = sps - s2.spd;
        s.acc = acc - s2.acc;
        s.eva = eva - s2.eva;
        return s;
    }
    
};

#endif    
