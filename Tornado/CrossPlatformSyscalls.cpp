#include "CrossPlatformSyscalls.h"

#define WINDOWS _WIN32 or _WIN64

#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

// I am not using std::this_thread::sleep_for() because this is supposed to work without the pthread lib
void cpSleep(std::size_t ms)
{
	#ifdef WINDOWS
		Sleep((DWORD)ms);
	#else
		sleep(ms);
	#endif;
	return;
}
