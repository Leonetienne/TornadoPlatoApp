#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <cstddef>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "resource.h"
#include "Implementation.h"
#include "renderres.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // Our instance
WCHAR szTitle[MAX_LOADSTRING];                  // Title of the main window
WCHAR szWindowClass[MAX_LOADSTRING];            // Class name of the main window

AppImplementation* appImplementation; // Our app

ATOM                RegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Custom code here
    appImplementation = new AppImplementation(RENDER_WIDTH, RENDER_HEIGHT);

    // Load strings from resources
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINWINDOWDISPLAY, szWindowClass, MAX_LOADSTRING);
    RegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINWINDOWDISPLAY));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



// Register window class
ATOM RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINWINDOWDISPLAY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

// Init instance
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
       CW_USEDEFAULT,
       0,
       RENDER_WIDTH,
       RENDER_HEIGHT,
       nullptr,
       nullptr,
       hInstance,
       nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Event handling
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 16, NULL);
        break;

    case WM_TIMER:
        appImplementation->Tick();
        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // Draw pixel buffer here!!
            BITMAPINFO bmi;
            memset(&bmi, 0, sizeof(bmi));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = RENDER_WIDTH;
            bmi.bmiHeader.biHeight = RENDER_HEIGHT;
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

                #pragma warning( push )
                #pragma warning( disable : 6386)
                memcpy(buffer, appImplementation->GetBGRPixelBuffer(), RENDER_WIDTH * RENDER_HEIGHT * 3);
                #pragma warning( pop ) 

                BitBlt(hdc, 0, 0, RENDER_WIDTH, RENDER_HEIGHT, hDibDC, 0, 0, SRCCOPY);

                DeleteObject(SelectObject(hDibDC, hOldObj));
            }
            
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        delete appImplementation;
        appImplementation = nullptr;
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
