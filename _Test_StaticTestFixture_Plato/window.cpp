#include "Window.h"
#include <functional>

Window::Window(const Vector2i& resolution, const std::string& title, const std::string& className)
    :
    resolution { resolution },
    title { title }
{
    // Create window class
    WNDCLASSA windowClass = { 0 };
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = NULL;
    windowClass.lpfnWndProc = Window::WndProc;
    windowClass.lpszClassName = className.c_str();
    windowClass.style = CS_HREDRAW | CS_VREDRAW;

    // Register window class
    if (!RegisterClassA(&windowClass))
        MessageBoxA(NULL, "Could not register class", "Error", MB_OK);

    // Finally create the window
    systemHandle = CreateWindowA(
        className.c_str(),
        title.c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        0,
        resolution.x,
        resolution.y,
        NULL,
        NULL,
        NULL,
        NULL
    );

    // Show window
    ShowWindow(systemHandle, SW_SHOW);
    
    // Insert into cross referencing table
    windows.insert(
        std::pair<HWND, Window*>(systemHandle, this)
    );
	
    return;
}

Window::~Window()
{
    if (isOpen)
        DestroyWindow(systemHandle);
    
    DeleteObject(systemHandle);

    return;
}

void Window::Poll()
{
    MSG messages;
    if (GetMessageA(&messages, NULL, 0, 0) > 0)
    {
        TranslateMessage(&messages);
        DispatchMessageA(&messages);
    }

    return;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hwnd, 1, 16, NULL);
        break;

    case WM_TIMER:
        //appImplementation->Tick();
        InvalidateRect(hwnd, NULL, false);
        break;

    case WM_PAINT:
    {
        //PAINTSTRUCT ps;
        //HDC hdc = BeginPaint(hwnd, &ps);
        //
        //// Draw pixel buffer here!!
        //BITMAPINFO bmi;
        //memset(&bmi, 0, sizeof(bmi));
        //bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        //bmi.bmiHeader.biWidth = ;
        //bmi.bmiHeader.biHeight = RENDER_HEIGHT;
        //bmi.bmiHeader.biPlanes = 1;
        //bmi.bmiHeader.biBitCount = 24;
        //bmi.bmiHeader.biCompression = BI_RGB;
        //
        //unsigned char* buffer;
        //HBITMAP hDib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
        //if (buffer == NULL)
        //    PostQuitMessage(-1);
        //if (hDib == NULL)
        //    PostQuitMessage(-2);
        //else
        //{
        //
        //    HDC hDibDC = CreateCompatibleDC(hdc);
        //    HGDIOBJ hOldObj = SelectObject(hDibDC, hDib);
        //
        //    #pragma warning( push )
        //    #pragma warning( disable : 6386)
        //    memcpy(buffer, appImplementation->GetBGRPixelBuffer(), RENDER_WIDTH * RENDER_HEIGHT * 3);
        //    #pragma warning( pop ) 
        //
        //    BitBlt(hdc, 0, 0, RENDER_WIDTH, RENDER_HEIGHT, hDibDC, 0, 0, SRCCOPY);
        //
        //    DeleteObject(SelectObject(hDibDC, hOldObj));
        //}
        //
        //EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        HwndToWindow(hwnd)->Cleanup();
       break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    default:
        return DefWindowProcA(hwnd, message, wparam, lparam);
    }
    
    return 0;
}

void Window::Cleanup()
{
    isOpen = false;
    windows.erase(systemHandle);

    return;
}

HWND Window::GetSystemWindowHandle() const
{
    return systemHandle;
}

bool Window::GetIsOpen() const
{
    return isOpen;
}

Window* Window::HwndToWindow(HWND hwnd)
{
    return windows.at(hwnd);
}

std::map<HWND, Window*> Window::windows;
