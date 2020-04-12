#pragma once
#include <string>
#include <iomanip>
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
	Arrow(const int _direction, const int heroX, const int heroY);
	~Arrow();
	static int arrowTotal;
	const int getX() const;
	const int getY() const;
	const char getShape() const;
	const void nextStep(int& nextX, int& nextY) const;
	const bool arrowMove();
	const bool isExist() const;
	const int damage(const int creatureX, const int creatureY);
	const void arrowEnd();

};