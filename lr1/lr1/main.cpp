#include "Windows.h"
#include "StaticImage.hpp"

#define RECTANGLE_ID_MENU 1001
#define ELLIPSE_ID_MENU 1002
#define IMAGE_ID_MENU 1003

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HMENU CreateMainMenu(HWND hwnd);
void selectedMenuItem(HMENU hMenu, int activeMenuItem, int clickedMenuItemId, StaticImage* staticImage);

const char g_szMainWindowClassName[] = "myWindowClass";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szMainWindowClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szMainWindowClassName,
        "Move the image",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
        NULL, NULL, hInst, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    CoInitialize(NULL);
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    CoUninitialize();
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    RECT rc;
    static StaticImage* staticImage;
    static HMENU hMenu;
    static HBRUSH hStaticBrush = CreateSolidBrush(RGB(255, 255, 255));
    static int activeMenuItem = IMAGE_ID_MENU;
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    int x = 0;
    int y = 0;    
    HWND hStatic;
    switch (msg) {
    case WM_CREATE:
        GetWindowRect(hwnd, &rc);
        x = width / 2 - (rc.right - rc.left) / 2;
        y = height / 2 - (rc.bottom - rc.top) / 2;
        SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        hStatic = CreateWindow(TEXT("static"), TEXT(""), WS_CHILD | WS_VISIBLE, 10, 10, 100, 100, hwnd, (HMENU)100, NULL, NULL);
        staticImage = new StaticImage(hwnd, hStatic, 10, 10, 100, 100);
        hMenu = CreateMainMenu(hwnd);
        activeMenuItem = IMAGE_ID_MENU;
        EnableMenuItem(hMenu, IMAGE_ID_MENU, MF_GRAYED);
        break;
    case WM_COMMAND:
        if (wparam == RECTANGLE_ID_MENU || wparam == ELLIPSE_ID_MENU || wparam == IMAGE_ID_MENU) selectedMenuItem(hMenu, activeMenuItem, wparam, staticImage);
        break;
    case WM_CTLCOLORSTATIC:
        return (LONG) hStaticBrush;   
    case WM_KEYDOWN:
        switch (wparam) {
        case VK_DOWN:
            staticImage->MoveDown();
            break;
        case VK_UP:
            staticImage->MoveUp();
            break;
        case VK_LEFT:
            staticImage->MoveLeft();
            break;
        case VK_RIGHT:
            staticImage->MoveRight();
            break;
        }        
        break;
    case WM_MOUSEWHEEL:
        GET_KEYSTATE_WPARAM(wparam) == MK_SHIFT ?
            GET_WHEEL_DELTA_WPARAM(wparam) > 0 ? staticImage->MoveRight() : staticImage->MoveLeft() :
            GET_WHEEL_DELTA_WPARAM(wparam) > 0 ? staticImage->MoveUp() : staticImage->MoveDown();        
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        if (staticImage) delete staticImage;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}

HMENU CreateMainMenu(HWND hwnd) {
    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, RECTANGLE_ID_MENU, TEXT("Rectangle"));
    AppendMenu(hSubMenu, MF_STRING, ELLIPSE_ID_MENU, TEXT("Ellipse"));
    AppendMenu(hSubMenu, MF_STRING, IMAGE_ID_MENU, TEXT("Image"));
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, TEXT("Drawing mode"));
    SetMenu(hwnd, hMenu);
    return hSubMenu;
}

void selectedMenuItem(HMENU hMenu, int activeMenuItem, int clickedMenuItemId, StaticImage* staticImage) {
    EnableMenuItem(hMenu, activeMenuItem, MF_ENABLED);
    EnableMenuItem(hMenu, clickedMenuItemId, MF_GRAYED);
    activeMenuItem = clickedMenuItemId;
    switch (clickedMenuItemId) {
    case RECTANGLE_ID_MENU:
        staticImage->SetDrawingMode(RECTANGLE);
        break;
    case ELLIPSE_ID_MENU:
        staticImage->SetDrawingMode(ELLIPSE);
        break;
    case IMAGE_ID_MENU:
        staticImage->SetDrawingMode(IMAGE);
        break;
    }    
    staticImage->Refresh();
}