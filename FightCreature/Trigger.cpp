#include "Trigger.h"

Trigger::Trigger(const int _x, const int _y) :x(_x), y(_y) {}

Trigger::~Trigger()
{

}

const int Trigger::getX() const
{
	return x;
}

const int Trigger::getY() const
{
	return y;
}

const int Trigger::getTrigger()
{
	exist = false;
	return exp;
}

const char Trigger::printTrigger() const
{
	return shape;
}

const bool Trigger::isExist() const
{
	return exist;
}