#include "Application.h"
#include "EventManager.h"

using namespace Plato;
using namespace Plato::Input;
using namespace Eule;

const Rect& Application::GetWindowRect()
{
	return EventManager::GetWindowRect();
}

const bool Application::Exit()
{
	return EventManager::ExecuteReverseEventCallback(
		REVERSE_EVENT_CALLBACK::EXIT,
		std::vector<double>()
	);
}
