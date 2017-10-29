#pragma once
#include <vector>
#include <sstream>
#include <string>
#include <Windows.h>

using namespace std;

class TableDrawer {
private:
    int cols;
    int rows;
    int colWidth;
    vector<vector<string>> text;
    vector<vector<HWND>> cells;
    vector<int> rowsHeight;
    HWND container;

    void loadTextFromString(string str) {
        int k = 0;
        vector<string> words;
        split(str, back_inserter(words));

        if (words.size() < cols * rows) return;

        vector<string> empty;
        for (int i = 0; i < rows; ++i) {
            empty.clear();
            text.push_back(empty);
            for (int j = 0; j < cols; ++j) {
                text[i].push_back(words[k++]);
            }
        }
    }

    template<typename Out>
    static vector<string> split(string str, Out result) {
        stringstream ss;
        ss.str(str);
        string item;
        while (getline(ss, item, ' ')) *(result++) = item;
    }   

    void calcSizes() {
        RECT rect;
        GetClientRect(container, &rect);
        colWidth = (rect.right - rect.left) / cols;

        HDC hdc = GetDC(container);
       
        rowsHeight.clear();
        int rowHeight = 0;
        for (int i = 0; i < rows; ++i) {
            rect.left = 0;
            rect.right = colWidth;
            rect.bottom = 0;
            rect.top = 0;
            rowHeight = 0;
            for (int j = 0; j < cols; ++j) {
                DrawText(hdc, text[i][j].c_str(), strlen(text[i][j].c_str()), &rect, DT_CALCRECT | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_NOPREFIX);
                if (rowHeight < rect.bottom - rect.top) {
                    rowHeight = rect.bottom - rect.top;
                }
                rect.left = 0;
                rect.right = colWidth;
                rect.bottom = 0;
                rect.top = 0;
            }
            rowsHeight.push_back(rowHeight + 4);
            rowHeight = 0;
        }
        ReleaseDC(container, hdc);
    }

    void removeStaticControls() {
        if (cells.size() > 0) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    DestroyWindow(cells[i][j]);
                }
            }
            cells.clear();
        }
    }
public:
    TableDrawer(int cols, int rows, HWND container) {
        this->cols = cols;
        this->rows = rows;
        this->container = container;
    }

    void setTextForDisaply(vector<vector<string>> data) {
        this->text = data;
    }

    void drawTable() {
        if (this->text.size() == 0) {
            MessageBox(container, "Set the text data", "Error", MB_ICONERROR);
            return;
        }
        removeStaticControls();
        calcSizes();
        vector<HWND> empty;
        HWND staticControl;
        int yPos = 0;
        for (int i = 0; i < rows; ++i) {
            empty.clear();
            for (int j = 0; j < cols; ++j) {
                staticControl = CreateWindowEx(WS_EX_STATICEDGE | WS_EX_NOPARENTNOTIFY, TEXT("static"), text[i][j].c_str(), WS_CHILD | WS_VISIBLE, j * colWidth, yPos, colWidth, rowsHeight[i], container, (HMENU)100, NULL, NULL);
                empty.push_back(staticControl);
            }
            yPos += rowsHeight[i];
            cells.push_back(empty);
        }
    }
};