#pragma once
#include <Windows.h>
#include <wincodec.h>

enum DRAWING_MODE {
    ELLIPSE = 1,
    RECTANGLE = 2,
    IMAGE = 3
};

enum MOVE_DIRECTION {
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4
};

class Drawer {
private:
    int x;
    int y;
    int width;
    int height;
    int speedX = 5;
    int speedY = 5;
    HWND container = NULL;
    HWND hStatic = NULL;
public:
    DRAWING_MODE drawingMode = IMAGE;
    MOVE_DIRECTION moveDirection = LEFT;
    MOVE_DIRECTION oldDirection = LEFT;
    int downX;
    int downY;
    BOOL moving = FALSE;
    int X() { return x; }
    int Y() { return y; }
    int Width() { return width; }
    int Height() { return height; }
    HWND Container() { return container; }

    Drawer(HWND container, HWND hStatic, int x, int y, int width, int height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->container = container;
        this->hStatic = hStatic;
    }
    void Draw() {
        RECT containerRect;
        GetWindowRect(container, &containerRect);
        if (x < 0) x = 10;
        if (x + width > containerRect.right - containerRect.left - 20) x = containerRect.right - containerRect.left - width - 30;
        if (y < 0) y = 10;
        if (y + height > containerRect.bottom - containerRect.top - 60) y = containerRect.bottom - containerRect.top - height - 70;
        if (oldDirection != moveDirection) {
            InvalidateRect(hStatic, NULL, TRUE);
            oldDirection = moveDirection;
        }
        MoveWindow(hStatic, x, y, width, height, TRUE);
    }
    void MoveUp() {
        moveDirection = UP;
        y -= speedY;
        Draw();
    }
    void MoveDown() {
        moveDirection = DOWN;
        y += speedY;
        Draw();
    }
    void MoveRight() {
        moveDirection = RIGHT;
        x += speedX;
        Draw();
    }
    void MoveLeft() {
        moveDirection = LEFT;
        x -= speedX;
        Draw();
    }
    void Move(int x, int y) {
        if (moving) {
            this->x = x;
            this->y = y;
            Draw();
        }
    }
    void SetSizes(int width, int height) {
        this->width = width;
        this->height = height;
        Draw();
    }
};