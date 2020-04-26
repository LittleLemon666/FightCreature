#pragma once
#include "Point.h"

class Trap
{
private:
	int x;
	int y;
	char shape = 'x';
	bool exist = true;

public:
	Trap(const int _x, const int _y, const char _shape);
	~Trap();
	static bool trapIsTouched;
	static int trapTotal;
	static char trapShape;
	const int getX() const;
	const int getY() const;
	const Point getPosition() const;
	const void touchTrap();
	const void destroyTrap(const bool haveDamage);
	const char printTrap() const;
	const bool isExist() const;
};