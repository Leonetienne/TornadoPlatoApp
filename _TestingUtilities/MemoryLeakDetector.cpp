#include "MemoryLeakDetector.h"

void MemoryLeakDetector::Init()
{
	_CrtMemCheckpoint(&stateInit);
	return;
}

bool MemoryLeakDetector::DetectLeak()
{
	_CrtMemCheckpoint(&stateEnd);
	return Evaluate();
}

int MemoryLeakDetector::Difference()
{
	_CrtMemCheckpoint(&stateEnd);
	return Evaluate();
}

int MemoryLeakDetector::Evaluate()
{
	return _CrtMemDifference(&stateBfr, &stateInit, &stateEnd);
}
