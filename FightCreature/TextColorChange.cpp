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