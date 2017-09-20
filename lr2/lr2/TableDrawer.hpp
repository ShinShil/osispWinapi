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
    int charWidth;
    vector<vector<string>> text;
    vector<vector<HWND>> cells;
    HWND container;
    void loadTextFromString(string str) {
        int k = 0;
        vector<string> words;
        split(str, back_inserter(words));
        if (words.size() < cols * rows) return;
        for (int i = 0; i < rows; ++i) for (int j = 0; j < cols; ++j) text[i][j] = words[k++];
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
        colWidth = rect.right - rect.left;
    }
public:
    TableDrawer(int cols, int rows, HWND container) {
        this->cols = cols;
        this->rows = rows;
        this->container = container;
    }

    

    void drawTable() {
        vector<HWND> empty;
        HWND staticControl;
        int cellHeight = 0; //personal for each row
        RECT rect;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                
                
                empty.push_back(staticControl);
            }
            cells.push_back(empty);
        }
    }

};