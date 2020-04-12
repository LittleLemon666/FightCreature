#include "Trap.h"

int Trap::trapTotal = 0;
bool Trap::trapIsTouched = false;
char Trap::trapShape = 'x';
Trap::Trap(const int _x, const int _y, const char _shape) :x(_x), y(_y), shape(_shape)
{
	trapShape = _shape;
	trapTotal++;
}

Trap::~Trap()
{

}

const int Trap::getX() const
{
	return x;
}

const int Trap::getY() const
{
	return y;
}

const void Trap::touchTrap()
{
	exist = false;
	trapIsTouched = true;
}

const void Trap::destroyTrap(const bool haveDamage)
{
	if (haveDamage)
	{
		exist = false;
	}
}

const char Trap::printTrap() const
{
	return shape;
}

const bool Trap::isExist() const
{
	return exist;
}