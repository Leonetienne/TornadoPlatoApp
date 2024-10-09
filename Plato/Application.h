#pragma once
#include "../Eule/Rect.h"

using namespace Eule;

namespace Plato
{
	namespace Input
	{
		/** Interface to interface with the application in general
		*/
		class Application
		{
		public:
			//! Will return the applications window bounds
			static const Eule::Rect& GetWindowRect();

			//! Will exit the application, if such an reverse-event callback is implemented.  
			//! Returns false if it isn't.
			static const bool Exit();
		};
	}
}
