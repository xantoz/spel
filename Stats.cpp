#include "Stats.hpp"
#include <sstream>

Stats Stats::operator-(const Stats &s2) const
{
    Stats s;
    s.maxhp = maxhp - s2.maxhp;
    s.str = str + s2.str;
    s.atk = atk - s2.atk;
    s.def = def - s2.def;
    s.spd = spd - s2.spd;
    s.acc = acc - s2.acc;
    s.eva = eva - s2.eva;
    return s;
}

Stats Stats::operator+(const Stats &s2) const 
{
    Stats s;
    s.maxhp = maxhp + s2.maxhp;
    s.str = str + s2.str;
    s.atk = atk + s2.atk;
    s.def = def + s2.def;
    s.spd = spd + s2.spd;
    s.acc = acc + s2.acc;
    s.eva = eva + s2.eva;
    return s;
}

Stats Stats::operator*(int scalar) const
{
    Stats s;
    s.maxhp = maxhp * scalar;
    s.str = str * scalar;
    s.atk = atk * scalar;
    s.def = def * scalar;
    s.spd = spd * scalar;
    s.acc = acc * scalar;
    s.eva = eva * scalar;
    return s;
}

Stats Stats::operator*(double scalar) const
{
    Stats s;
    s.maxhp = (int) (maxhp * scalar);
    s.str = (int) (str * scalar);
    s.atk = (int) (atk * scalar);
    s.def = (int) (def * scalar);
    s.spd = (int) (spd * scalar);
    s.acc = (int) (acc * scalar);
    s.eva = (int) (eva * scalar);
    return s;
}


Stats Stats::operator/(int scalar) const
{
    Stats s;
    s.maxhp = maxhp / scalar;
    s.str = str / scalar;
    s.atk = atk / scalar;
    s.def = def / scalar;
    s.spd = spd / scalar;
    s.acc = acc / scalar;
    s.eva = eva / scalar;
    return s;
}

Stats &Stats::operator+=(const Stats &s2)
{
    maxhp += s2.maxhp;
    str += s2.str;
    atk += s2.atk;
    def += s2.def;
    spd += s2.spd;
    acc += s2.acc;
    eva += s2.eva;
    return *this;
}

Stats &Stats::operator-=(const Stats &s2)
{
    maxhp -= s2.maxhp;
    str -= s2.str;
    atk -= s2.atk;
    def -= s2.def;
    spd -= s2.spd;
    acc -= s2.acc;
    eva -= s2.eva;
    return *this;
}

Stats &Stats::operator+=(int scalar)
{
    maxhp += scalar;
    str += scalar;
    atk += scalar;
    def += scalar;
    spd += scalar;
    acc += scalar;
    eva += scalar;
    return *this;
}

Stats &Stats::operator-=(int scalar)
{
    maxhp -= scalar;
    str -= scalar;
    atk -= scalar;
    def -= scalar;
    spd -= scalar;
    acc -= scalar;
    eva -= scalar;
    return *this;
}

std::string Stats::toString() const
{
    std::ostringstream os;
    os << " MAXHP: " << maxhp
       << " STR: " << str
       << " ATK: " << atk
       << " DEF: " << def
       << " SPD: " << spd
       << " ACC: " << acc
       << " EVA: " << eva;
    return os.str();
}

std::string Stats::serializeString() const
{
    std::ostringstream os;
    os << maxhp << ";" << str << ";" << atk << ";" << def << ";" << spd << ";" << acc << ";" << eva;
}

    
