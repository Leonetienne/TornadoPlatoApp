#include "Mouse.h"
#include "EventManager.h"

const Vector2i& Input::Mouse::GetGlobalMousePosition()
{
	return EventManager::GetGlobalMousePosition();
}

const Vector2i& Input::Mouse::GetLocalMousePosition()
{
	return EventManager::GetLocalMousePosition();
}

double Input::Mouse::GetMousewheelDelta()
{
	return EventManager::GetMousewheelDelta();
}

bool Input::Mouse::SetGlobalMousePosition(const Vector2i& newPos)
{
	return EventManager::ExecuteReverseEventCallback(
		REVERSE_EVENT_CALLBACK::SET_GLOBAL_MOUSE_POSITION,
		std::vector<double>({
			(double)newPos.x,
			(double)newPos.y
		})
	);
}

bool Input::Mouse::SetLocalMousePosition(const Vector2i& newPos)
{
	return EventManager::ExecuteReverseEventCallback(
		REVERSE_EVENT_CALLBACK::SET_LOCAL_MOUSE_POSITION,
		std::vector<double>({
			(double)newPos.x,
			(double)newPos.y
		})
	);
}
