#include "Keyboard.h"
#include "EventManager.h"

bool Input::Keyboard::GetKey(KEY_CODE key)
{
	// Is the key being pressed at all?

	// key state can either be held-down or just-pressed
	const KEY_STATE& keyState = EventManager::GetKeyState(key);
	return (keyState == KEY_STATE::HELD) || (keyState == KEY_STATE::DOWN);
}

bool Input::Keyboard::GetKeyDown(KEY_CODE key)
{
	// Has the key just been pressed?
	return EventManager::GetKeyState(key) == KEY_STATE::DOWN;
}

bool Input::Keyboard::GetKeyUp(KEY_CODE key)
{
	// Has the key just been released?
	return EventManager::GetKeyState(key) == KEY_STATE::UP;
}
