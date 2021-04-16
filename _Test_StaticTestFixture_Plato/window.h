#pragma once
#include <Windows.h>
#include <map>
#include "../Tornado/Vector2.h"

class Window
{
public:
	Window(const Vector2i& resolution, const std::string& title, std::string className = "");
	~Window();

	HWND GetSystemWindowHandle() const;
	void Poll();

	bool GetIsOpen() const;

	static Window* HwndToWindow(HWND hwnd);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	// Call after destroying window
	void Cleanup();

	HWND systemHandle; // Window handle
	const Vector2i resolution;
	const std::string title;
	bool isOpen = true;

	// This is to cross-reference from hwnd to window-class, like in the WndProc
	static std::map<HWND, Window*> windows;
};
