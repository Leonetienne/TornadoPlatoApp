#include "Application.h"
#include "EventManager.h"

const Rect& Input::Application::GetWindowRect()
{
	return EventManager::GetWindowRect();
}

const bool Input::Application::Exit()
{
	return EventManager::ExecuteReverseEventCallback(
		REVERSE_EVENT_CALLBACK::EXIT,
		std::vector<double>()
	);
}
