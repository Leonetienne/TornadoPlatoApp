#include "RenderWindow.h"
#include <functional>

RenderWindow::RenderWindow(const Vector2i& resolution, const std::string& title, std::string className)
    :
    resolution { resolution },
    title { title },
    pixelBuffer { nullptr }
{
    // Substitute for default class name
    if (className.length() == 0)
        this->className = title;

    // Create and poll Windows window in seperate thread
    eventPoller = new std::thread(&RenderWindow::Thread__ThreadLife, this);

    // Create bitblp compatible pixel buffer buffer
    bgrPixelBuffer = new uint8_t[(std::size_t)resolution.x * (std::size_t)resolution.y * (std::size_t)3];

    return;
}

RenderWindow::~RenderWindow()
{
    if (isOpen)
        DestroyWindow(systemHandle);
    
    eventPoller->join();
    delete eventPoller;
    eventPoller = nullptr;

    DeleteObject(systemHandle);

    delete[] bgrPixelBuffer;
    bgrPixelBuffer = nullptr;

    return;
}

void RenderWindow::Thread__ThreadLife()
{
    // Create window
    Thread__CreateWindow();

    // Listen to window events
    Thread__PollEvents();

    return;
}

void RenderWindow::Thread__CreateWindow()
{
    // Create window class
    WNDCLASSA windowClass = { 0 };
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = NULL;
    windowClass.lpfnWndProc = RenderWindow::WndProc;
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
        std::pair<HWND, RenderWindow*>(systemHandle, this)
    );

    return;
}

void RenderWindow::Thread__PollEvents()
{
    MSG messages;
    while (GetMessageA(&messages, systemHandle, 0, 0) > 0)
    {
        TranslateMessage(&messages);
        DispatchMessageA(&messages);
    }

    return;
}

LRESULT CALLBACK RenderWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hwnd, 1, 16, NULL);
        break;

    case WM_TIMER:
        InvalidateRect(hwnd, NULL, false);
        break;

    case WM_PAINT:
    {
        RenderWindow* self = HwndToWindow(hwnd);
        
        // No pixelbuffer, no paint!
        if (self->pixelBuffer == nullptr)
            return 0;

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        // Draw pixel buffer here!!
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = self->resolution.x;
        bmi.bmiHeader.biHeight = self->resolution.y;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 24;
        bmi.bmiHeader.biCompression = BI_RGB;
        
        unsigned char* buffer;
        HBITMAP hDib = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
        if (buffer == NULL)
            PostQuitMessage(-1);
        if (hDib == NULL)
            PostQuitMessage(-2);
        else
        {
        
            HDC hDibDC = CreateCompatibleDC(hdc);
            HGDIOBJ hOldObj = SelectObject(hDibDC, hDib);
            
            self->drawingMutex.lock();
            #pragma warning( push )
            #pragma warning( disable : 6386)
            memcpy(buffer, self->bgrPixelBuffer, (std::size_t)self->resolution.x * (std::size_t)self->resolution.y * (std::size_t)3);
            #pragma warning( pop ) 
            self->drawingMutex.unlock();

            BitBlt(hdc, 0, 0, self->resolution.x, self->resolution.y, hDibDC, 0, 0, SRCCOPY);
        
            DeleteObject(SelectObject(hDibDC, hOldObj));
        }
        
        EndPaint(hwnd, &ps);
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

void RenderWindow::Cleanup()
{
    isOpen = false;
    windows.erase(systemHandle);

    return;
}

void RenderWindow::UpdateBgrPixelBuffer()
{
    drawingMutex.lock();

    for (std::size_t i = 0; i < pixelBuffer->GetSizeofBuffer(); i += 3)
    {
        bgrPixelBuffer[i + 0] = pixelBuffer->GetRawData()[i + 2];
        bgrPixelBuffer[i + 1] = pixelBuffer->GetRawData()[i + 1];
        bgrPixelBuffer[i + 2] = pixelBuffer->GetRawData()[i + 0];
    }

    drawingMutex.unlock();

    return;
}

HWND RenderWindow::GetSystemWindowHandle() const
{
    return systemHandle;
}

bool RenderWindow::GetIsOpen() const
{
    return isOpen;
}

RenderWindow* RenderWindow::HwndToWindow(HWND hwnd)
{
    return windows.at(hwnd);
}

void RenderWindow::SetTitle(const std::string& title)
{
    SetWindowTextA(systemHandle, title.c_str());
    return;
}

void RenderWindow::SetPixelBuffer(const PixelBuffer<3>* pxb)
{
    pixelBuffer = pxb;
    return;
}

std::map<HWND, RenderWindow*> RenderWindow::windows;
