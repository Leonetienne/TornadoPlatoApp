#pragma once
#include <crtdbg.h>

class MemoryLeakDetector
{
public:
	// Defines the initial mem state to check against
	void Init();

	// Returns whether or not the process uses more memory than at the point when Init() was called
	bool DetectLeak();

	// Returns the absolute memory difference since calling Init(). WARNING: The ABSOLUTE difference! No negative values!
	int Difference();


private:
	int Evaluate();

	_CrtMemState stateInit;
	_CrtMemState stateEnd;
	_CrtMemState stateBfr;

};
