#include "EventManager.h"

void EventManager::Init()
{
	InitializeCharacterMapping();

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

void EventManager::RegisterEventNewWindowRect(const Rect& windowRect)
{
	// Don't apply the event right now, but rather at the end of the frame.
	eventQueue.push_back([windowRect]() {
		EventManager::windowRect = windowRect;
	});

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

KEY_STATE EventManager::GetKeyState(KEY_CODE key) const
{
	// Key not found
	if (keyStates.find(key) == keyStates.end())
		return KEY_STATE::NONE;

	// Return state
	return keyStates[key];
}

double EventManager::GetMousewheelDelta() const
{
	return mousewheelDelta;
}

Vector2i EventManager::GetLocalMousePosition() const
{
	return localMousePosition;
}

Vector2i EventManager::GetGlobalMousePosition() const
{
	return globalMousePosition;
}

Rect EventManager::GetWindowRect() const
{
	return windowRect;
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

	return;
}

void EventManager::InitializeCharacterMapping()
{
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
Rect EventManager::windowRect;
double EventManager::mousewheelDelta;
