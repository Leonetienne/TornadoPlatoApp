#include "Mouse.h"
#include "EventManager.h"

using namespace Plato;
using namespace Plato::Input;

const Vector2i& Mouse::GetGlobalMousePosition()
{
	return EventManager::GetGlobalMousePosition();
}

const Vector2i& Mouse::GetLocalMousePosition()
{
	return EventManager::GetLocalMousePosition();
}

const Vector2i& Mouse::GetMouseDelta()
{
	return EventManager::GetMouseDelta();
}

double Mouse::GetMousewheelDelta()
{
	return EventManager::GetMousewheelDelta();
}

bool Mouse::SetGlobalMousePosition(const Vector2i& newPos)
{
	return EventManager::ExecuteReverseEventCallback(
		REVERSE_EVENT_CALLBACK::SET_GLOBAL_MOUSE_POSITION,
		std::vector<double>({
			(double)newPos.x,
			(double)newPos.y
		})
	);
}

bool Mouse::SetLocalMousePosition(const Vector2i& newPos)
{
	return EventManager::ExecuteReverseEventCallback(
		REVERSE_EVENT_CALLBACK::SET_LOCAL_MOUSE_POSITION,
		std::vector<double>({
			(double)newPos.x,
			(double)newPos.y
		})
	);
}
