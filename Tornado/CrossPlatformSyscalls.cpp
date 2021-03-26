#include "CrossPlatformSyscalls.h"

#define WINDOWS _WIN32 or _WIN64

#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

inline void Sleep(std::size_t ms)
{
	#ifdef WINDOWS
		Sleep(ms);
	#else
		sleep(ms);
	#endif;
	return;
}
