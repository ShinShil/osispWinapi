#pragma once
#include <Windows.h>

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
        MoveWindow(hStatic, x, y, width, height, TRUE);
    }
    void MoveUp() {
        y -= speedY;
        Draw();
    }
    void MoveDown() {
        y += speedY;
        Draw();
    }
    void MoveRight() {
        x += speedX;
        Draw();
    }
    void MoveLeft() {
        x -= speedX;
        Draw();
    }
    void Move(int x, int y) {
            this->x = x;
            this->y = y;
            Draw();
    }
};