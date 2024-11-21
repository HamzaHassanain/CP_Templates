#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
// Geometry
struct Frac
{
    ll nom, denom;
    Frac()
    {
        nom = 0;
        denom = 1;
    }
    Frac(ll n, ll d) : nom(n), denom(d)
    {
        ll g = __gcd(nom, denom);
        nom /= g;
        denom /= g;
    }
    void operator=(const Frac &other)
    {
        nom = other.nom;
        denom = other.denom;
    }
    bool operator<(const Frac &other) const
    {
        return nom * other.denom < other.nom * denom;
    }
};
Frac operator*(const Frac &a, const Frac &b)
{
    ll denom = a.denom * b.denom;
    ll nom = a.nom * b.nom;
    ll g = __gcd(nom, denom);
    return Frac(nom / g, denom / g);
}
Frac operator/(const Frac &a, const Frac &b)
{
    ll denom = a.denom * b.nom;
    ll nom = a.nom * b.denom;
    ll g = __gcd(nom, denom);

    assert(denom != 0);

    return Frac(nom / g, denom / g);
}
Frac operator+(const Frac &a, const Frac &b)
{
    ll denom = a.denom * b.denom;
    ll nom = a.nom * b.denom + b.nom * a.denom;
    ll g = __gcd(nom, denom);
    return Frac(nom / g, denom / g);
}
Frac operator-(const Frac &a, const Frac &b)
{
    ll denom = a.denom * b.denom;
    ll nom = (a.nom * b.denom - b.nom * a.denom);
    ll g = __gcd(nom, denom);
    return Frac(nom / g, denom / g);
}
void FixZero(Frac &f)
{
    if (f.nom == 0)
        f.denom = 1;
}
