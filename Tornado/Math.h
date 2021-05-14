#pragma once

class Math
{
public:
	static constexpr double Max(double a, double b);
	static constexpr double Min(double a, double b);
	static constexpr double Clamp(double v, double min, double max);
	static constexpr double Lerp(double a, double b, double t);

private:
	// No instanciation! >:(
	Math();
};

constexpr inline double Math::Max(double a, double b)
{
	return (a > b) ? a : b;
}

constexpr inline double Math::Min(double a, double b)
{
	return (a < b) ? a : b;
}

constexpr inline double Math::Clamp(double v, double min, double max)
{
	return Max(Min(v, max), min);
}

constexpr inline double Math::Lerp(double a, double b, double t)
{
	double it = 1.0 - t;
	return (a * it) + (b * t);
}
