#pragma once
#include <chrono>

/** A class to measure time easy and precisely
*/
class Clock
{
public:
	//! Duration representation in various units
	class Duration
	{
	public:
		//! Get duration as seconds
		double AsSeconds() const;

		//! Get duration as milliseconds
		double AsMilliseconds() const;

		//! Get duration as microseconds
		double AsMicroseconds() const;

		//! Get duration as nanoseconds
		double AsNanoseconds() const;

	private:
		Duration(const std::chrono::duration<double>& tp);

		std::chrono::duration<double> duration;

		friend class Clock;
	};

public:
	Clock();
	~Clock();

	//! Will reset the clock
	void Reset();

	//! Will return the elapsed time since either the instanciation of this object, or the last call of Reset()
	Duration GetElapsedTime() const;

private:
	std::chrono::steady_clock::time_point begin;
};

