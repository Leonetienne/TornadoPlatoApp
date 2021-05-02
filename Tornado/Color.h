#pragma once
#include <cstdint>
#include <ostream>

/** Fluent floating-point color representation.
* Color ranges from 0-255. Support for linear interpolation, and comparison.  
* Also contains a few pre-defined colors.
*/
struct Color
{
	Color() : r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 } {};
	Color(double r, double g, double b, double a = 255);

	double r;
	double g;
	double b;
	double a;

	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;

	//! Will lerp itself to other by t
	void LerpSelf(const Color& other, double t);
	//! Will lerp between self and other
	Color Lerp(const Color& other, double t) const;
	//! Will lerp between a and b
	static Color Lerp(const Color& a, const Color& b, double t);

	friend std::ostream& operator<< (std::ostream& os, const Color& c)
	{
		return os << "[r: " << c.r << ", g: " << c.g << ", b: " << c.b << ", a: " << c.a << "]";
	}

	friend std::wostream& operator<< (std::wostream& os, const Color& c)
	{
		return os << L"[r: " << c.r << L", g: " << c.g << L", b: " << c.b << L", a: " << c.a << L"]";
	}

	static const Color black;
	static const Color gray;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color yellow;
	static const Color cyan;
	static const Color pink;
	static const Color white;
};
