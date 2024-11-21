#include <bits/stdc++.h>

using namespace std;

#include <complex>

#define PI acos(-1)
#define EPS 1e-9

#define point complex<double>
#define x real()
#define y imag()

#define polar(r, theta) (r) * exp(point(0, (theta)))

#define angle(v) (atan2((v).y, (v).x))
#define angleToVector(t) (polar(1, t))
#define vec(a, b) ((b) - (a))
#define length(v) ((double)hypot((v).y, (v).x))
#define dot(a, b) ((conj(a) * (b)).x)   // a*b*cos(theta)
#define cross(a, b) ((conj(a) * (b)).y) // a*b*sin(theta)
#define lengthSqr(v) (dot(v, v))
#define same(a, b) (dot(vec(a, b), vec(a, b)) < EPS)
#define normalize(v) ((v) / length(v))
#define norm(a) (conj(a) * (a)).x

#define rotate0(v, t) ((v) * exp(point(0, t)))
#define rotateA(p, a, t) (rotate0(vec(a, p), t) + a)
#define reflect0(v, m) (conj((v) / (m)) * (m))

int double_compare(double a, double b)
{
    return fabs(a - b) <= EPS ? 0 : a < b ? -1
                                          : 1;
}

point reflect(point p, point p0, point p1)
{
    point z = p - p0, w = p1 - p0;
    return conj(z / w) * w + p0;
}

bool is_collinear(point a, point b, point c)
{
    return fabs(cross(b - a, c - a)) < EPS;
}

bool is_point_on_ray(point p0, point p1, point p2)
{
    if (not is_collinear(p0, p1, p2))
        return false;
    return double_compare(dot(p1 - p0, p2 - p0), 0) == 1;
}
bool is_point_on_ray2(point p0, point p1, point p2)
{
    if (length(p2 - p0) < EPS)
        return true;

    return same(normalize(p1 - p0), normalize(p2 - p0));
}

bool is_point_on_segment(point a, point b, point c)
{
    return is_point_on_ray(a, b, c) and is_point_on_ray(b, a, c);
}

double dist_to_line(point p0, point p1, point p2)
{
    return fabs(cross(p1 - p0, p2 - p0) / length(p0 - p1));
}
double dist_to_segment(point p0, point p1, point p2)
{
    double d1, d2;
    point v1 = p1 - p0, v2 = p2 - p0;
    if ((d1 = dot(v1, v2)) <= 0)
        return length(p2 - p0);
    if ((d2 = dot(v1, v1)) <= d1)
        return length(p2 - p1);

    double d = d1 / d2;
    return length(p2 - (p0 + v1 * d));
}

bool intersect_segments(point a, point b, point c, point d, point &intersect)
{
    double d1 = cross(a - b, d - c), d2 = cross(a - c, d - c), d3 = cross(a - b, a - c);

    if (fabs(d1) < EPS)
        return false;

    double t1 = d2 / d1, t2 = d3 / d1;

    intersect = a + (b - a) * t1;

    if (t1 < -EPS or t2 < -EPS or t2 > 1 + EPS)
        return false;

    return true;
}
int counterClockWise(point p0, point p1, point p2)
{
    point v1(p1 - p0), v2(p2 - p0);
    if (cross(v1, v2) > EPS)
        return 1;
    if (cross(v1, v2) < -EPS)
        return -1;
    if (v1.x * v2.x < 0 or v1.y * v2.y < 0)
        return -1;

    if (norm(v1) < norm(v2) - EPS)
        return 1;

    return 0;
}
int clockWise(point p0, point p1, point p2)
{
    return -counterClockWise(p0, p1, p2);
}
bool intersect_segments_handle_point(point p1, point p2, point p3, point p4)
{

    bool X = (p1 == p2), Y = (p3 == p4);

    if (X and Y)
        return p1 == p3;
    if (X)
        return counterClockWise(p3, p4, p1) == 0;
    if (Y)
        return counterClockWise(p1, p2, p3) == 0;

    return (counterClockWise(p1, p2, p3) * counterClockWise(p1, p2, p4) <= 0) and (counterClockWise(p3, p4, p1) * counterClockWise(p3, p4, p2) <= 0);
}
