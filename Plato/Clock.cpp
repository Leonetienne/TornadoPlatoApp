#include "Clock.h"

using namespace Plato;

double Clock::Duration::AsSeconds() const
{
	return AsNanoseconds() * 10e-10;
}

double Clock::Duration::AsMilliseconds() const
{
	return AsNanoseconds() * 10e-7;
}

double Clock::Duration::AsMicroseconds() const
{
	return AsNanoseconds() * 10e-4;
}

double Clock::Duration::AsNanoseconds() const
{
	return (double)std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

Clock::Duration::Duration(const std::chrono::duration<double>& tp)
{
	duration = tp;
	return;
}



Clock::Clock()
{
	Reset();
	return;
}

Clock::~Clock()
{

	return;
}

void Clock::Reset()
{
	begin = std::chrono::high_resolution_clock::now();
	return;
}

Clock::Duration Clock::GetElapsedTime() const
{
	return Duration(
		std::chrono::high_resolution_clock::now() - begin
	);
}
