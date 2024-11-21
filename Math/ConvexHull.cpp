#include <bits/stdc++.h>
using namespace std;

typedef complex<double> point;
#define EPS 1e-8;
#define X real()
#define Y imag()
#define vec(a, b) ((b) - (a))
#define cp(a, b) ((conj(a) * (b)).imag())
#define sz(v) ((int)((v).size()))

bool com(point a, point b)
{
    return a.X < b.X || (a.X == b.X && a.Y < b.Y);
}
bool cw(point a, point b, point c)
{
    return cp(vec(a, b), vec(b, c)) <= 0;
}
bool ccw(point a, point b, point c)
{
    return cp(vec(a, b), vec(b, c)) >= 0;
}
vector<point> convex_hull(vector<point> &p)
{
    if (sz(p) == 1)
        return p;
    sort(p.begin(), p.end(), &com);
    point p1 = p[0];
    point p2 = p.back();
    vector<point> up, down;

    up.emplace_back(p1);
    down.emplace_back(p1);

    for (int i = 1; i < (int)p.size(); i++)
    {
        if (i == sz(p) - 1 or cw(p1, p[i], p2))
        {
            while (sz(up) >= 2 && !cw(up[sz(up) - 2], up[sz(up) - 1], p[i]))
                up.pop_back();
            up.push_back(p[i]);
        }
        if (i == sz(p) - 1 or ccw(p1, p[i], p2))
        {
            while (sz(down) >= 2 && !ccw(down[sz(down) - 2], down[sz(down) - 1], p[i]))
                down.pop_back();
            down.push_back(p[i]);
        }
    }
    vector<point> convex;
    for (int i = 0; i < sz(down); i++)
    {
        if (sz(convex) and convex.back() == down[i])
            continue;
        convex.push_back(down[i]);
    }
    for (int i = sz(up) - 2; i >= 0; i--)
    {
        if (sz(convex) and convex.back() == up[i])
            continue;
        convex.push_back(up[i]);
    }
    return convex;
}