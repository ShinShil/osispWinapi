#pragma once
#include "Drawer.hpp"
#include "StaticImage.hpp"
#include <Windows.h>
#include <vector>
#include <Objidl.h>

#pragma comment (lib, "Windowscodecs.lib")

static WNDPROC prevStaticWndProc;
static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class StaticImage {
private:
    HWND hStatic;
    Drawer* drawer;
public:
    HWND GetStatic() { return hStatic; }
    Drawer* GetDrawer() { return drawer; }
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
    void Refresh() {
        InvalidateRect(hStatic, NULL, TRUE);
    }
    void SetDrawingMode(DRAWING_MODE mode) {
        drawer->drawingMode = mode;
        if (mode == RECTANGLE || mode == ELLIPSE) drawer->SetSizes(100, 100);
    }
    static void LoadImageWithRotation(WCHAR* pathToImage, HDC hdc, Drawer* drawer) {
        IWICImagingFactory *pFactory = NULL;

        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IWICImagingFactory,
            (LPVOID*)&pFactory
        );

        IWICBitmapDecoder *pIDecoder = NULL;
        IWICBitmapFrameDecode *pIDecoderFrame = NULL;
        IWICBitmapFlipRotator *pIFlipRotator = NULL;
        
        hr = pFactory->CreateDecoderFromFilename(
            pathToImage,
            NULL,
            GENERIC_READ | GENERIC_WRITE,
            WICDecodeMetadataCacheOnDemand,
            &pIDecoder);
        
        hr = pIDecoder->GetFrame(0, &pIDecoderFrame);
        hr = pFactory->CreateBitmapFlipRotator(&pIFlipRotator);
        
        IWICBitmapSource *pSource = NULL;
        WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, pIDecoderFrame, &pSource);
        
        switch (drawer->moveDirection) {
        case UP:
            hr = pIFlipRotator->Initialize(pSource, WICBitmapTransformRotate90); 
            break;
        case LEFT:
            hr = pIFlipRotator->Initialize(pSource, WICBitmapTransformRotate0);
            break;
        case RIGHT:
            hr = pIFlipRotator->Initialize(pSource, WICBitmapTransformFlipHorizontal);
            break;
        case DOWN:
            hr = pIFlipRotator->Initialize(pSource, WICBitmapTransformRotate270);
            break;

        }

        UINT width = 0, height = 0;
        pIFlipRotator->GetSize(&width, &height);
        std::vector<BYTE> buffer(width * height * 4);
        pIFlipRotator->CopyPixels(0, width * 4, buffer.size(), buffer.data());

        HBITMAP hImageBitmap = CreateBitmap(width, height, 1, 32, buffer.data());
        HDC hdcMem = CreateCompatibleDC(hdc);
        HGDIOBJ oldBitmap = SelectObject(hdcMem, hImageBitmap);
        BITMAP bitmap;
        GetObject(hImageBitmap, sizeof(bitmap), &bitmap);
        drawer->SetSizes(bitmap.bmWidth, bitmap.bmHeight);
        BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
        DeleteObject(hImageBitmap);
        if (pFactory) pFactory->Release();
        if (pIDecoder) pIDecoder->Release();
    }
};

static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    (*prevStaticWndProc)(hwnd, msg, wparam, lparam);
    Drawer* drawer = (Drawer*)GetProp(hwnd, TEXT("drawer"));
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
    case WM_SETCURSOR:        
        SetCursor(LoadCursor(NULL, IDC_HAND));
        return TRUE;
    case WM_MOUSEMOVE:
        x = drawer->X() + (LOWORD(lparam) - drawer->downX);
        y = drawer->Y() + (HIWORD(lparam) - drawer->downY);
        drawer->Move(x, y);
        return TRUE;
    case WM_PAINT:
        HDC hdc = GetDC(hwnd);
        HPEN hpenOld = static_cast<HPEN>(SelectObject(hdc, GetStockObject(DC_PEN)));
        HBRUSH hbrushOld = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(DC_BRUSH)));
        SetDCPenColor(hdc, RGB(255, 0, 0));
        SetDCBrushColor(hdc, RGB(255, 0, 0));
        if (drawer->drawingMode == ELLIPSE) {
            Ellipse(hdc, 0, 0, drawer->Width(), drawer->Height());
        }
        else if (drawer->drawingMode == RECTANGLE) {
            Rectangle(hdc, 0, 0, drawer->Width(), drawer->Height());
        }
        else if (drawer->drawingMode == IMAGE) {
            StaticImage::LoadImageWithRotation(L"D:\\dog.jpg", hdc, drawer);
        }
        SelectObject(hdc, hpenOld);
        SelectObject(hdc, hbrushOld);
        ReleaseDC(hwnd, hdc);
        return TRUE;
    }
    return TRUE;
}