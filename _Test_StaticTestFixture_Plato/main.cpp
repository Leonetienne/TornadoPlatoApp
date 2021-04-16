#include "Window.h"

int main()
{
	Window window({ 1920, 1080 }, "Plato Static Test Fixture");

	while (window.GetIsOpen())
		window.Poll();

	return 0;
}