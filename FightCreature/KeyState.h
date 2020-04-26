#pragma once
#include <conio.h>		// _getch() <= to read input without enter "ENTER" key

enum KeyState
{
    ESC,
    W,
    S,
    A,
    D,
    SPACE,
    J,
    UA,
    DA,
    LA,
    RA,
    ENTER,
    ZERO,
    ONE,
    TWO,
    THREE,
    INVALID
};

const void getKey(bool key[]);

const void clearInputBuffer();