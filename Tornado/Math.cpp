#include "Math.h"

double Math::Max(double a, double b)
{
    return (a > b) ? a : b;
}

double Math::Min(double a, double b)
{
    return (a < b) ? a : b;
}

double Math::Clamp(double v, double min, double max)
{
    return Max(Min(v, max), min);
}

double Math::Lerp(double a, double b, double t)
{
    double it = 1.0 - t;
    return (a * it) + (b * t);
}
