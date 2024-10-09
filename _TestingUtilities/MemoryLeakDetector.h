#pragma once
#include <crtdbg.h>

// IIRC this works on windows visual c++ only
// Don't even allow compilation in release mode unless handled correctly.
// This class ONLY WORKS IN DEBUG MODE!!!
#ifdef _DEBUG
class MemoryLeakDetector
{
public:
	// Defines the initial mem state to check against
	void Init()
	{
		_CrtMemCheckpoint(&stateInit);
		return;
	};

	// Returns whether or not the process uses more memory than at the point when Init() was called
	bool DetectLeak()
	{
		_CrtMemCheckpoint(&stateEnd);
		return Evaluate();
	};

	// Returns the absolute memory difference since calling Init(). WARNING: The ABSOLUTE difference! No negative values!
	int Difference()
	{
		// I know that it's the exact same code as in DetectedLeak().
		_CrtMemCheckpoint(&stateEnd);
		return Evaluate();
	}


private:
	int Evaluate()
	{
		return _CrtMemDifference(&stateBfr, &stateInit, &stateEnd);
	}

	_CrtMemState stateInit;
	_CrtMemState stateEnd;
	_CrtMemState stateBfr;

};
#endif
