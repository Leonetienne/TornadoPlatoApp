#pragma once
#include <Windows.h>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "../Plato/Vector.h"
#include "../Tornado/PixelBuffer.h"

using namespace Plato;

class RenderWindow
{
public:
	RenderWindow(const Vector2i& resolution, const std::string& title, std::string className = "");
	~RenderWindow();

	HWND GetSystemWindowHandle() const;

	bool GetIsOpen() const;
	void Close();

	static RenderWindow* HwndToWindow(HWND hwnd);

	void SetTitle(const std::string& title);
	void SetPixelBuffer(const TorGL::PixelBuffer<3>* pxb);

	// Will update the bgr pixel buffer from the rgb pixel buffer
	void UpdateBgrPixelBuffer();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	// Call after destroying window
	void Cleanup();

	HWND systemHandle; // Window handle
	const Vector2i resolution;
	std::string title;
	std::string className;
	bool isOpen = true;
	const TorGL::PixelBuffer<3>* pixelBuffer;
	// The windows api takes bgr instead of rgb -.-
	uint8_t* bgrPixelBuffer;

	std::thread* eventPoller;
	void Thread__ThreadLife();
	void Thread__CreateWindow();
	void Thread__PollEvents();
	std::mutex drawingMutex; // Poor man's v-sync, but between the renderer and the window

	// This is to cross-reference from hwnd to window-class, like in the WndProc
	static std::unordered_map<HWND, RenderWindow*> windows;
};
