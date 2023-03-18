#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cassert>
#include <iomanip>

#include "GL\glut.h"

const int     max_int = (std::numeric_limits<int>::max)();
const double  max_double = (std::numeric_limits<double>::max)();
const double  min_double = (std::numeric_limits<double>::min)();
const float   max_float = (std::numeric_limits<float>::max)();
const float   min_float = (std::numeric_limits<float>::min)();

const double   PI = 3.14159;
const double   two_PI = PI * 2;
const double   half_PI = PI / 2;
const double   quarter_PI = PI / 4;

template <typename T>
inline bool is_nan(T val)
{
    return val != val;
}

inline double degs_to_rads(double degs)
{
    return two_PI * (degs / 360.0);
}

inline bool IsZero(double val)
{
    return ((-min_double < val) && (val < min_double));
}

inline bool InRange(double start, double end, double val)
{
    if (start < end)
    {
        if ((val > start) && (val < end)) return true;
        else return false;
    }

    else
    {
        if ((val < start) && (val > end)) return true;
        else return false;
    }
}

template <class T>
T Maximum(const T& v1, const T& v2)
{
    return v1 > v2 ? v1 : v2;
}

inline int   rand_int(int x, int y)
{
    assert(y >= x && "<RandInt>: y is less than x");
    return rand() % (y - x + 1) + x;
}

inline double rand_float() { return ((rand()) / (RAND_MAX + 1.0)); }

inline double rand_in_range(double x, double y)
{
    return x + rand_float() * (y - x);
}

inline bool   rand_bool()
{
    if (rand_float() > 0.5) return true;

    else return false;
}

inline double random_clamped() { return rand_float() - rand_float(); }

inline double rand_gaussian(double mean = 0.0, double standard_deviation = 1.0)
{
    double x1, x2, w, y1;
    static double y2;
    static int use_last = 0;

    if (use_last)
    {
        y1 = y2;
        use_last = 0;
    }
    else
    {
        do
        {
            x1 = 2.0 * rand_float() - 1.0;
            x2 = 2.0 * rand_float() - 1.0;
            w = x1 * x1 + x2 * x2;
        } while (w >= 1.0);

        w = sqrt((-2.0 * log(w)) / w);
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }

    return(mean + y1 * standard_deviation);
}

inline double sigmoid(double input, double response = 1.0)
{
    return (1.0 / (1.0 + exp(-input / response)));
}

template <class T>
inline T max_of(const T& a, const T& b)
{
    if (a > b) return a; return b;
}

template <class T>
inline T min_of(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template <class T, class U, class V>
inline void clamp(T& arg, const U& minVal, const V& maxVal)
{
    assert(((double)minVal < (double)maxVal) && "<Clamp>MaxVal < MinVal!");

    if (arg < (T)minVal)
    {
        arg = (T)minVal;
    }

    if (arg > (T)maxVal)
    {
        arg = (T)maxVal;
    }
}

inline int rounded(double val)
{
    int    integral = (int)val;
    double mantissa = val - integral;

    if (mantissa < 0.5)
    {
        return integral;
    }

    else
    {
        return integral + 1;
    }
}

inline int round_under_offset(double val, double offset)
{
    int    integral = (int)val;
    double mantissa = val - integral;

    if (mantissa < offset)
    {
        return integral;
    }

    else
    {
        return integral + 1;
    }
}

inline bool is_equal(float a, float b)
{
    if (fabs(a - b) < 1E-12)
    {
        return true;
    }

    return false;
}

inline bool is_equal(double a, double b)
{
    if (fabs(a - b) < 1E-12)
    {
        return true;
    }

    return false;
}

template <class T>
inline double average(const std::vector<T>& v)
{
    double average = 0.0;

    for (unsigned int i = 0; i < v.size(); ++i)
    {
        average += (double)v[i];
    }

    return average / (double)v.size();
}

inline double standard_deviation(const std::vector<double>& v)
{
    double sd = 0.0;
    double avg = average(v);

    for (unsigned int i = 0; i < v.size(); ++i)
    {
        sd += (v[i] - avg) * (v[i] - avg);
    }

    sd = sd / v.size();

    return sqrt(sd);
}

//template <class container>
//inline void delete_stl_container(container& c)
//{
//    for (container::iterator it = c.begin(); it != c.end(); ++it)
//    {
//        delete* it;
//        *it = NULL;
//    }
//}

//template <class map>
//inline void delete_stl_map(map& m)
//{
//    for (map::iterator it = m.begin(); it != m.end(); ++it)
//    {
//        delete it->second;
//        it->second = NULL;
//    }
//}

inline float f_random_range(int min, int max) {

    float r3 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    return r3;
}
#endif