#include "EventManager.h"

using namespace Plato;
using namespace Plato::Input;
using namespace Eule;

void EventManager::Init()
{
	InitializeKeys();

	return;
}

bool EventManager::RegisterEventKeyDown(KEY_CODE key)
{
	// Check if that key actually exists
	if (keyStates.find(key) == keyStates.end())
		return false;

	// Don't apply the event right now, but rather at the end of the frame.
	eventQueue.push_back([key]() {
		keyStates[key] = KEY_STATE::DOWN; // Set key to 'just been pressed down'
	});

	return true;
}

bool EventManager::RegisterEventKeyUp(KEY_CODE key)
{
	// Check if that key actually exists
	if (keyStates.find(key) == keyStates.end())
		return false;

	// Don't apply the event right now, but rather at the end of the frame.
	eventQueue.push_back([key]() {
		keyStates[key] = KEY_STATE::UP; // Set key to 'just been released down'
	});

	return true;
}

void EventManager::RegisterEventMousewheelDelta(double mwdelta)
{
	// Don't apply the event right now, but rather at the end of the frame.
	eventQueue.push_back([mwdelta]() {
		mousewheelDelta = mwdelta;
	});

	return;
}

void EventManager::RegisterEventMousePosition(const Vector2i& globalPos, const Vector2i& localPos)
{
	// Don't apply the event right now, but rather at the end of the frame.
	eventQueue.push_back([globalPos, localPos]() {

		globalMousePosition = globalPos;
		if (localPos != Vector2i(-1, -1))
			localMousePosition = localPos;
		else
		{
			// No local position supplied? Deduce it from the client area
			localMousePosition.x = globalPos.x - (int)windowRect.pos.x;
			localMousePosition.y = globalPos.y - (int)windowRect.pos.y;
		}

	});
	return;
}

void EventManager::RegisterEventMouseDelta(const Vector2i& mouseDelta)
{
	// Don't apply the event right now, but rather at the end of the frame.
	eventQueue.push_back([mouseDelta]() {

		mouseDeltaPosition = mouseDelta;

	});
	return;
}

void EventManager::RegisterEventNewWindowRect(const Rect& windowRect)
{
	// Don't apply the event right now, but rather at the end of the frame.
	eventQueue.push_back([windowRect]() {
		EventManager::windowRect = windowRect;
	});

	return;
}

void EventManager::RegisterReverseEventCallback(REVERSE_EVENT_CALLBACK evnt, std::function<void(std::vector<double>)> callback)
{
	reverseEventCallbacks.insert(
		std::pair<REVERSE_EVENT_CALLBACK, std::function<void(std::vector<double>)>>(evnt, callback)
	);

	return;
}

void EventManager::Digest()
{
	// Execute tick
	Tick();

	// Execute event queue
	for (const std::function<void()>& ev : eventQueue)
		ev();

	// Clear event queue
	eventQueue.clear();

	return;
}

KEY_STATE EventManager::GetKeyState(KEY_CODE key)
{
	// Key not found
	if (keyStates.find(key) == keyStates.end())
		return KEY_STATE::NONE;

	// Return state
	return keyStates[key];
}

double EventManager::GetMousewheelDelta()
{
	return mousewheelDelta;
}

const Vector2i& EventManager::GetLocalMousePosition()
{
	return localMousePosition;
}

const Vector2i& EventManager::GetGlobalMousePosition()
{
	return globalMousePosition;
}

const Vector2i& EventManager::GetMouseDelta()
{
	return mouseDeltaPosition;
}

const Rect& EventManager::GetWindowRect()
{
	return windowRect;
}

bool EventManager::ExecuteReverseEventCallback(REVERSE_EVENT_CALLBACK evnt, const std::vector<double>& params)
{
	// Is this event-callback implemented?
	if (reverseEventCallbacks.find(evnt) == reverseEventCallbacks.end())
		return false;

	// Call reverse-event callback
	reverseEventCallbacks[evnt](params);
	return true;
}

void EventManager::Tick()
{
	// Update key states
	for (auto& ks : keyStates)
	{
		// Transfer just-pressed-down keys to held-keys
		if (ks.second == KEY_STATE::DOWN)
			ks.second = KEY_STATE::HELD;

		// Transfer just-released keys to released keys
		if (ks.second == KEY_STATE::UP)
			ks.second = KEY_STATE::RELEASED;
	}

	// Clear mouse wheel data
	mousewheelDelta = 0.0;

	// Clear mouse data
	mouseDeltaPosition = Vector2i(0,0);

	return;
}

void EventManager::InitializeKeys()
{
	// Initialize key states
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::MOUSE_L, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::MOUSE_R, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::MOUSE_M, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::MOUSE_X1, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::MOUSE_X2, KEY_STATE::RELEASED));

	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::A, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::B, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::C, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::D, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::E, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::G, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::H, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::I, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::J, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::K, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::L, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::M, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::N, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::O, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::P, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::Q, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::R, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::S, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::T, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::U, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::V, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::W, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::X, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::Y, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::Z, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_0, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_1, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_2, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_3, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_4, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_5, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_6, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_7, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_8, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUM_9, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_0, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_1, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_2, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_3, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_4, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_5, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_6, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_7, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_8, KEY_STATE::RELEASED));

	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::NUMPAD_NLOCK, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::DIVIDE, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::MULTIPLY, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::SUBTRACT, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::ADD, KEY_STATE::RELEASED));

	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::PERIOD, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::MINUS, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::COMMA, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::PLUS, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::SPACE, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::CIRCUMFLEX, KEY_STATE::RELEASED));

	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::ARROW_UP, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::ARROW_DOWN, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::ARROW_LEFT, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::ARROW_RIGHT, KEY_STATE::RELEASED));

	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F1, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F2, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F3, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F4, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F5, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F6, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F7, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F8, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F9, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F10, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F11, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F12, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F13, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F14, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F15, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F16, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F17, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F18, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F19, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F20, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F21, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F22, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F23, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::F24, KEY_STATE::RELEASED));

	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::ESCAPE, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::BACK, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::TAB, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::LSHIFT, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::RSHIFT, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::LCONTROL, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::RCONTROL, KEY_STATE::RELEASED));
    keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::LALT, KEY_STATE::RELEASED));
    keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::RALT, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::CAPSLOCK, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::INSERT, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::DEL, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::PRINT, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::PAUSE, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::SCROLL_LOCK, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::PAGE_UP, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::PAGE_DOWN, KEY_STATE::RELEASED));
	keyStates.insert(std::pair<KEY_CODE, KEY_STATE>(KEY_CODE::END, KEY_STATE::RELEASED));

	// Initialize key mappings
	RegisterCharacterMapping('a', KEY_CODE::A);
	RegisterCharacterMapping('b', KEY_CODE::B);
	RegisterCharacterMapping('c', KEY_CODE::C);
	RegisterCharacterMapping('d', KEY_CODE::D);
	RegisterCharacterMapping('e', KEY_CODE::E);
	RegisterCharacterMapping('f', KEY_CODE::F);
	RegisterCharacterMapping('g', KEY_CODE::G);
	RegisterCharacterMapping('h', KEY_CODE::H);
	RegisterCharacterMapping('i', KEY_CODE::I);
	RegisterCharacterMapping('j', KEY_CODE::J);
	RegisterCharacterMapping('k', KEY_CODE::K);
	RegisterCharacterMapping('l', KEY_CODE::L);
	RegisterCharacterMapping('m', KEY_CODE::M);
	RegisterCharacterMapping('n', KEY_CODE::N);
	RegisterCharacterMapping('o', KEY_CODE::O);
	RegisterCharacterMapping('p', KEY_CODE::P);
	RegisterCharacterMapping('q', KEY_CODE::Q);
	RegisterCharacterMapping('r', KEY_CODE::R);
	RegisterCharacterMapping('s', KEY_CODE::S);
	RegisterCharacterMapping('t', KEY_CODE::T);
	RegisterCharacterMapping('u', KEY_CODE::U);
	RegisterCharacterMapping('v', KEY_CODE::V);
	RegisterCharacterMapping('w', KEY_CODE::W);
	RegisterCharacterMapping('x', KEY_CODE::X);
	RegisterCharacterMapping('y', KEY_CODE::Y);
	RegisterCharacterMapping('z', KEY_CODE::Z);
	RegisterCharacterMapping(' ', KEY_CODE::SPACE);
	RegisterCharacterMapping('0', KEY_CODE::NUM_0);
	RegisterCharacterMapping('1', KEY_CODE::NUM_1);
	RegisterCharacterMapping('2', KEY_CODE::NUM_2);
	RegisterCharacterMapping('3', KEY_CODE::NUM_3);
	RegisterCharacterMapping('4', KEY_CODE::NUM_4);
	RegisterCharacterMapping('5', KEY_CODE::NUM_5);
	RegisterCharacterMapping('6', KEY_CODE::NUM_6);
	RegisterCharacterMapping('7', KEY_CODE::NUM_7);
	RegisterCharacterMapping('8', KEY_CODE::NUM_8);
	RegisterCharacterMapping('9', KEY_CODE::NUM_9);
	RegisterCharacterMapping(',', KEY_CODE::COMMA);
	RegisterCharacterMapping('.', KEY_CODE::PERIOD);
	RegisterCharacterMapping('-', KEY_CODE::MINUS);
	RegisterCharacterMapping('+', KEY_CODE::ADD);
	RegisterCharacterMapping('/', KEY_CODE::DIVIDE);
	RegisterCharacterMapping('*', KEY_CODE::MULTIPLY);
	RegisterCharacterMapping('^', KEY_CODE::CIRCUMFLEX);
	return;
}

void EventManager::RegisterCharacterMapping(char ch, KEY_CODE key)
{
	char_keycode_mapping.insert(std::pair<char, KEY_CODE>(ch, key));
	keycode_char_mapping.insert(std::pair<KEY_CODE, char>(key, ch));

	return;
}

std::vector<std::function<void()>> EventManager::eventQueue;
std::unordered_map<KEY_CODE, KEY_STATE> EventManager::keyStates;
std::unordered_map<KEY_CODE, char> EventManager::keycode_char_mapping;
std::unordered_map<char, KEY_CODE> EventManager::char_keycode_mapping;
Vector2i EventManager::localMousePosition;
Vector2i EventManager::globalMousePosition;
Vector2i EventManager::mouseDeltaPosition;
Rect EventManager::windowRect;
double EventManager::mousewheelDelta;
std::unordered_map<REVERSE_EVENT_CALLBACK, std::function<void(std::vector<double>)>> EventManager::reverseEventCallbacks;
