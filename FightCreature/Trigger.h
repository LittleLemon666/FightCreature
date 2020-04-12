#pragma once
#include <iomanip>

class Trigger
{
private:
	int x;
	int y;
	int exp = 1;
	char shape = '+';
	bool exist = true;
	static const int defaultTriggerGenerateTimeFrame = 2000;
	static clock_t triggerTimeBegin;

public:
	Trigger(const int _x, const int _y);
	~Trigger();
	static int triggerTotal;
	const int getX() const;
	const int getY() const;
	const int getTrigger();
	static const bool canGenerate();
	const char printTrigger() const;
	const bool isExist() const;
};