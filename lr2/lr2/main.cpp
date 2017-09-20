#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    char *text = "Determines the width and height of the rectangle. \
        If there are multiple lines of text, DrawText uses the width of the \
        rectangle pointed to by the lpRect parameter and extends the base \
        of the rectangle to bound the last line of text. If the largest word \
        is wider than the rectangle, the width is expanded. If the text is less \
        than the width of the rectangle, the width is reduced. If there is only \
        one line of text, DrawText modifies the right side of the rectangle so that \
        it bounds the last character in the line. In either case, DrawText returns \
        the height of the formatted text but does not draw the text.";

    int MenuAndWndFrameHeight = 100;

    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    RECT r;
    int xDelta;     // xDelta = new_pos - current_pos  
    int xNewPos;    // new position 
    int yDelta = 0;

    switch (message)
    {
    case WM_CREATE:
        r.left = 0;
        r.right = 25;
        r.bottom = 0;
        r.top = 0;
        hdc = GetDC(hWnd);
        DrawText(hdc, text, strlen(text), &r, DT_CALCRECT | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_NOPREFIX);
        CreateWindowEx(WS_EX_STATICEDGE,TEXT("static"), text, WS_CHILD | WS_VISIBLE, 0, 0, 25, r.bottom - r.top + 32, hWnd, (HMENU)100, NULL, NULL);
        CreateWindowEx(WS_EX_STATICEDGE,TEXT("static"), text, WS_CHILD | WS_VISIBLE, 100, 0, 25, r.bottom - r.top + 32, hWnd, (HMENU)100, NULL, NULL);

        break;
    case WM_VSCROLL:

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        //// TODO: добавьте любой код отрисовки...
        /*RECT r;
        r.left = 0;
        r.right = 100;
        r.bottom = 0;
        r.top = 0;*/

        //DrawText(hdc, text, strlen(text), &r, DT_CALCRECT | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_NOPREFIX);
        //DrawText(hdc, text, strlen(text), &r, DT_LEFT | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_NOPREFIX);

        //SetWindowPos(hWnd, NULL, r.left, r.top, r.right - r.left + 25, r.bottom - r.top + MenuAndWndFrameHeight, 0);

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}