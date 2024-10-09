#pragma once
#include "KeyState.h"
#include "KeyCode.h"
#include "Event_ReverseEventCallback.h"
#include "Vector.h"
#include "../Eule/Rect.h"
#include <unordered_map>
#include <functional>
#include <vector>

namespace Plato
{
	//! This namespace contains implementations to interface with the user and system
	namespace Input
	{
		/** User input handler.
		* Translate and forward system events to this class. This class determines what various user input modules
		* report.
		*
		* Reverse-Event callbacks are functionalities implemented by the application end-point such as setting the cursor position, as these are highly platform-dependent
		*/
		class EventManager
		{
		public:
			static void Init();

			//! Will register a key being pressed down  
			//! Will return false if they key is unknown
			static bool RegisterEventKeyDown(KEY_CODE key);

			//! Will register a key being pressed up  
			//! Will return false if they key is unknown
			static bool RegisterEventKeyUp(KEY_CODE key);

			//! Will register the mouse wheel being turned
			static void RegisterEventMousewheelDelta(double mwdelta);

			//! Will register a new cursor position.  
			//! If no value for localPos is supplied, it will be deduced by the client area.
			static void RegisterEventMousePosition(const Vector2i& globalPos, const Vector2i& localPos = Vector2i(-1, -1));
    //
			//! Will register a new cursor delta.  
			static void RegisterEventMouseDelta(const Vector2i& mouseDelta);

			//! Will register any change in the window rect, such as a window movement or resizing.
			static void RegisterEventNewWindowRect(const Eule::Rect& windowRect);

			//! Will register a reverse event callback.  
			//! This provides the game engine with functionality, such as setting the cursor position.
			static void RegisterReverseEventCallback(REVERSE_EVENT_CALLBACK event, std::function<void(std::vector<double>)> callback);

			//! Will digest all events registered since the last call of Digest().  
			//! Call at the end of each frame.
			static void Digest();

			//! Will return the state of a given key
			static KEY_STATE GetKeyState(KEY_CODE key);

			//! Will return the delta-movement of the mouse wheel
			static double GetMousewheelDelta();

			//! Will return the mouse position relative to the window rect
			static const Vector2i& GetLocalMousePosition();

			//! Will return the absolute mouse position
			static const Vector2i& GetGlobalMousePosition();

			//! Will return the mouse delta
			static const Vector2i& GetMouseDelta();

			//! Will return the window rectangle
			static const Eule::Rect& GetWindowRect();

			//! Will execute a reverse-event callback.  
			//! Returns false if this reverse-event is not implemented
			static bool ExecuteReverseEventCallback(REVERSE_EVENT_CALLBACK evnt, const std::vector<double>& params);

		private:
			// Does iterative work, like switching the state of just-down keys to the held-state
			static void Tick();

			// Store events in this, to digest at the end of the frame
			static std::vector<std::function<void()>> eventQueue;

			static void InitializeKeys();
			static void RegisterCharacterMapping(char ch, KEY_CODE key);

			static Vector2i localMousePosition;
			static Vector2i globalMousePosition;
			static Vector2i mouseDeltaPosition;
			static Eule::Rect windowRect;
			static double mousewheelDelta;

			static std::unordered_map<KEY_CODE, KEY_STATE> keyStates;
			static std::unordered_map<KEY_CODE, char> keycode_char_mapping;
			static std::unordered_map<char, KEY_CODE> char_keycode_mapping;

			static std::unordered_map<REVERSE_EVENT_CALLBACK, std::function<void(std::vector<double>)>> reverseEventCallbacks;

			// No instanciating!
			EventManager();
		};
	}
}
