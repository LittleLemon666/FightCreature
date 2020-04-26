#include "KeyState.h"

const void getKey(bool key[])
{
    for (int i = 0; i < INVALID; i++)
    {
        key[i] = false;
    }
    char inputKey = _getch();
    switch (inputKey)
    {
    case 'w':
    case 'W':
        key[W] = true;
        break;
    case 's':
    case 'S':
        key[S] = true;
        break;
    case 'a':
    case 'A':
        key[A] = true;
        break;
    case 'd':
    case 'D':
        key[D] = true;
        break;
    case ' ':
        key[SPACE] = true;
        break;
    case 'j':
    case 'J':
        key[J] = true;
        break;
    case 27:
        key[ESC] = true;
        break;
    case 13:
        key[ENTER] = true;
        break;
    case 72:
        key[UA] = true;
        break;
    case 80:
        key[DA] = true;
        break;
    case 75:
        key[LA] = true;
        break;
    case 77:
        key[RA] = true;
        break;
    case '0':
        key[ZERO] = true;
        break;
    case '1':
        key[ONE] = true;
        break;
    case '2':
        key[TWO] = true;
        break;
    case '3':
        key[THREE] = true;
        break;
    }
}

const void clearInputBuffer()
{
    while (_kbhit())
    {
        _getch();
    }
}