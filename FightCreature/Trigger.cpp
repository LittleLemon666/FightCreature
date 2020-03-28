#include "Trigger.h"

Trigger::Trigger(int _x, int _y)
{
	x = _x;
	y = _y;
}

Trigger::~Trigger()
{

}

int Trigger::getX()
{
	return x;
}

int Trigger::getY()
{
	return y;
}

int Trigger::getTrigger()
{
	exist = false;
	return exp;
}

char Trigger::printTrigger()
{
	return shape;
}

bool Trigger::isExist()
{
	return exist;
}