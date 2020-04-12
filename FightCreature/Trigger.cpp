#include "Trigger.h"

int Trigger::triggerTotal = 0;
clock_t Trigger::triggerTimeBegin = clock();
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

const bool Trigger::canGenerate()
{
	if (clock() - triggerTimeBegin > defaultTriggerGenerateTimeFrame)
	{
		triggerTimeBegin = clock();
		return true;
	}
	return false;
}

const char Trigger::printTrigger() const
{
	return shape;
}

const bool Trigger::isExist() const
{
	return exist;
}