#pragma once
#include "Drawer.hpp"
#include "StaticImage.hpp"
#include <Windows.h>

static WNDPROC prevStaticWndProc;
static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class StaticImage {
private:
    HWND hStatic;
    Drawer* drawer;
public:
    StaticImage(HWND container, HWND hStatic, int x, int y, int width, int height) {
        this->hStatic = hStatic;
        this->drawer = new Drawer(container, hStatic, x, y, width, height);
        prevStaticWndProc = (WNDPROC)SetWindowLongPtr(this->hStatic, GWLP_WNDPROC, (LONG_PTR)StaticWndProc);
        SetProp(hStatic, TEXT("drawer"), (HANDLE)this->drawer);
        InvalidateRect(hStatic, NULL, TRUE);
    }
    void MoveUp() {
        drawer->MoveUp();
    }
    void MoveDown() {
        drawer->MoveDown();
    }
    void MoveRight() {
        drawer->MoveRight();
    }
    void MoveLeft() {
        drawer->MoveLeft();
    }
    void Move(int x, int y) {
        drawer->Move(x, y);
    }
};

static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    (*prevStaticWndProc)(hwnd, msg, wparam, lparam);
    Drawer* drawer = (Drawer*)GetProp(hwnd, TEXT("drawer"));
    TCHAR buffer[256];
    int x = 0, y = 0;
    switch (msg) {
    case WM_LBUTTONDOWN:
        drawer->downX = LOWORD(lparam);
        drawer->downY = HIWORD(lparam);
        drawer->moving = TRUE;
        return TRUE;
    case WM_LBUTTONUP:
        drawer->moving = FALSE;
        return TRUE;
    case WM_MOUSEMOVE:
        x = drawer->X() + (LOWORD(lparam) - drawer->downX);
        y = drawer->Y() + (HIWORD(lparam) - drawer->downY);
        drawer->Move(x, y);
        return TRUE;
    case WM_PAINT:
        HDC hdc = GetDC(hwnd);
        HPEN hpenOld = static_cast<HPEN>(SelectObject(hdc, GetStockObject(DC_PEN)));
        HBRUSH hbrushOld = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(NULL_BRUSH)));
        SetDCPenColor(hdc, RGB(255, 0, 0));
        Rectangle(hdc, 0, 0, drawer->Width(), drawer->Height());
        SelectObject(hdc, hpenOld);
        SelectObject(hdc, hbrushOld);
        ReleaseDC(hwnd, hdc);
        return TRUE;
    }

    return TRUE;
}