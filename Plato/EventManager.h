#pragma once
#include "KeyState.h"
#include "KeyCode.h"
#include "../Tornado/Vector2.h"
#include "../Tornado/Rect.h"
#include <unordered_map>
#include <functional>
#include <vector>

/** User input handler.
* Translate and forward system events to this class. This class determines what various user input modules
* report.
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

	//! Will register any change in the window rect, such as a window movement or resizing.
	static void RegisterEventNewWindowRect(const Rect& windowRect);

	//! Will digest all events registered since the last call of Digest().  
	//! Call at the end of each frame.
	static void Digest();

	//! Will return the state of a given key
	KEY_STATE GetKeyState(KEY_CODE key) const;

	//! Will return the delta-movement of the mouse wheel
	double GetMousewheelDelta() const;

	//! Will return the mouse position relative to the window rect
	Vector2i GetLocalMousePosition() const;
	
	//! Will return the absolute mouse position
	Vector2i GetGlobalMousePosition() const;

	//! Will return the window rectangle
	Rect GetWindowRect() const;

private:
	// Does iterative work, like switching the state of just-down keys to the held-state
	static void Tick();
	
	// Store events in this, to digest at the end of the frame
	static std::vector<std::function<void()>> eventQueue;

	static void InitializeCharacterMapping();
	static void RegisterCharacterMapping(char ch, KEY_CODE key);

	static Vector2i localMousePosition;
	static Vector2i globalMousePosition;
	static Rect windowRect;
	static double mousewheelDelta;

	static std::unordered_map<KEY_CODE, KEY_STATE> keyStates;
	static std::unordered_map<KEY_CODE, char> keycode_char_mapping;
	static std::unordered_map<char, KEY_CODE> char_keycode_mapping;

	// No instanciating!
	EventManager();
};
