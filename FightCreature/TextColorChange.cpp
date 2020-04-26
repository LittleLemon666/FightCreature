#include "TextColorChange.h"

void TextColor(int fontcolor, int backgroundcolor, HANDLE screen)
{
    //Author: Brian The Mad
    //URL: https://www.daniweb.com/programming/software-development/threads/9921/change-text-color-using-visual-c
    int color_attribute;
    color_attribute = backgroundcolor;
    color_attribute = _rotl(color_attribute, 4) | fontcolor;
    SetConsoleTextAttribute(screen, color_attribute);
}

const void fontsize(const int a, const int b, HANDLE out)
{
    //URL: http://mycodecollection.blogspot.com/2015/01/c-console-change-font-size.html
    PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
    lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
    lpConsoleCurrentFontEx->dwFontSize.X = a;
    lpConsoleCurrentFontEx->dwFontSize.Y = b;
    SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}