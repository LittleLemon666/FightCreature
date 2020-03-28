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
	Trigger(int _x, int _y);
	~Trigger();
	int getX();
	int getY();
	int getTrigger();
	char printTrigger();
	bool isExist();
};