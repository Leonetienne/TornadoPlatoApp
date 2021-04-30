#pragma once
#include <chrono>

class Clock
{
public:
	class Duration
	{
	public:
		double AsSeconds() const;
		double AsMilliseconds() const;
		double AsMicroseconds() const;
		double AsNanoseconds() const;

	private:
		Duration(const std::chrono::duration<double>& tp);

		std::chrono::duration<double> duration;

		friend class Clock;
	};

public:
	Clock();
	~Clock();

	void Reset();
	Duration GetElapsedTime() const;

private:
	std::chrono::steady_clock::time_point begin;
};

