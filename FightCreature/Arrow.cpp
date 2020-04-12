#include "Arrow.h"
#include "Direction.h"
using namespace std;

int Arrow::arrowTotal = 0;
Arrow::Arrow(const int _direction, const int heroX, const int heroY) : direction(_direction), x(heroX + dx[_direction]), y(heroY + dy[_direction])
{
	arrowTotal++;
}

Arrow::~Arrow()
{

}

const int Arrow::getX() const
{
	return x;
}

const int Arrow::getY() const
{
	return y;
}

const char Arrow::getShape() const
{
	return shape[direction];
}

const void Arrow::nextStep(int& nextX, int& nextY) const
{
	nextX = x + dx[direction];
	nextY = y + dy[direction];
}

const bool Arrow::arrowMove()
{
	if (clock() - shootBegin > defaultArrowMoveSpeed)
	{
		x += dx[direction];
		y += dy[direction];
		return true;
	}
	return false;
}

const bool Arrow::isExist() const
{
	return exist;
}

const int Arrow::damage(const int creatureX, const int creatureY)
{
	if (x == creatureX && y == creatureY)
	{
		exist = false;
		return attack;
	}
	return 0;
}

const void Arrow::arrowEnd()
{
	exist = false;
}