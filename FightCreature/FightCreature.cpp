#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Chapter.h"
using namespace std;

const int defaultKeyNum = 16;
bool key[defaultKeyNum] = { false };
Chapter chapter;

const void maximumWindows();
const bool menu();

int main()
{
    srand(time(NULL));
    maximumWindows();

    if (!menu())
    {
        return 0;
    }

    chapter.gaming(key);

    return 0;
}

const void maximumWindows()
{
    //Author: ashishchoure
    //URL: https://www.daniweb.com/programming/software-development/threads/308396/setting-the-console-window-size
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1920, 1080, TRUE);
}

const bool menu()
{
    cout << "Quick game please press 1.\n";
    cout << "Load game please press 2.\n";
    cout << "Custom game please press 3.\n";
    cout << "Press ESC to leave.\n";
    int result;
    while (true)
    {
        getKey(key);
        if (key[ONE])
        {
            chapter.quickGame();
            return true;
        }
        else if (key[TWO])
        {
            result = chapter.loadGame(key);
            if (result == 0)
            {
                menu();
            }
            return true;
        }
        else if (key[THREE])
        {
            chapter.customGame();
            return true;
        }
        else if (key[ESC])
        {
            return false;
        }
    }
}