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

