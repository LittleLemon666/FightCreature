#pragma once
class Trigger
{
private:
	int x;
	int y;
	int exp = 1;
	char shape = '+';
	bool exist = true;

public:
	Trigger(const int _x, const int _y);
	~Trigger();
	static int triggerTotal;
	const int getX() const;
	const int getY() const;
	const int getTrigger();
	const char printTrigger() const;
	const bool isExist() const;
};