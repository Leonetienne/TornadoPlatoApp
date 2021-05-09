#pragma once
#include "../Tornado/Rect.h"

namespace Input
{
	/** Interface to interface with the application in general
	*/
	class Application
	{
	public:
		//! Will return the applications window bounds
		static const Rect& GetWindowRect();

		//! Will exit the application, if such an reverse-event callback is implemented.  
		//! Returns false if it isn't.
		static const bool Exit();
	};
}
