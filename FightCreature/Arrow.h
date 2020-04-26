#pragma once
#include <string>
#include <iomanip>
#include "Point.h"
using namespace std;

class Arrow
{
private:
	int x;
	int y;
	int direction;
	int defaultArrowMoveSpeed = 100;
	int attack = 1;
	string shape = "^v<>";
	clock_t shootBegin = clock();
	bool exist = true;

public:
	Arrow(const int _direction, const Point heroPosition);
	~Arrow();
	static int arrowTotal;
	const int getX() const;
	const int getY() const;
	const char getShape() const;
	const void nextStep(int& nextX, int& nextY) const;
	const bool arrowMove();
	const bool isExist() const;
	const int damage(const Point creaturePosition);
	const void arrowEnd();

};