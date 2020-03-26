#include <iostream>
#include <vector>
#include <sstream>
#include "Creature.h"
using namespace std;

void Creature::setCreatureLocation(int dungeonWidth, int dungeonHeight, int mode)
{
	switch (mode)
	{
	case 1:
		x = rand() % (dungeonWidth - 2) + 1;
		y = rand() % (dungeonHeight - 2) + 1;
		break;
	case 2:
		break;
	case 3:
		cout << "\n";
		cout << "Set creature's location\n";
		bool valid;
		do
		{
			valid = true;
			cout << "x (1 ~ " << dungeonWidth - 2 << "): ";
			cin >> x;
			if (x < 1 || x >= dungeonWidth - 1)
			{
				valid = false;
				cout << "input is out of range (1 ~ " << dungeonWidth - 2 << ")\n";
			}
		} while (!valid);

		do
		{
			valid = true;
			cout << "y (1 ~ " << dungeonHeight - 2 << "): ";
			cin >> y;
			if (y < 1 || y >= dungeonHeight - 1)
			{
				valid = false;
				cout << "input is out of range (1 ~ " << dungeonHeight - 2 << ")\n";
			}
		} while (!valid);
		break;
	}
}

void Creature::setCreatureLocation(int _x, int _y)
{
	x = _x;
	y = _y;
}

int Creature::setCreatureInformation(vector<string>& stringLine, int lineIndex)
{
	stringstream ss;
	ss << stringLine[lineIndex++];
	ss >> health >> attack >> energy >> exp;
	return lineIndex;
}

void Creature::seeHero(int heroX, int heroY)
{
	if (x > heroX && x - heroX <= 3 && y == heroY)
	{
		heroDirection = West;
	}
	else if (x < heroX && x - heroX >= -3 && y == heroY)
	{
		heroDirection = East;
	}
	else if (x > heroX && x - heroX <= 3 && y > heroY && y - heroY <= 3)
	{
		heroDirection = NorthWest;
	}
	else if (x > heroX && x - heroX <= 3 && y < heroY && y - heroY >= -3)
	{
		heroDirection = SouthWest;
	}
	else if (x < heroX && x - heroX >= -3 && y > heroY && y - heroY <= 3)
	{
		heroDirection = NorthEast;
	}
	else if (x < heroX && x - heroX >= -3 && y < heroY && y - heroY >= -3)
	{
		heroDirection = SouthEast;
	}
	else if (x == heroX && y < heroY && y - heroY >= -3)
	{
		heroDirection = South;
	}
	else if (x == heroX && y > heroY && y - heroY <= 3)
	{
		heroDirection = North;
	}
	else if (x == heroX && y == heroY)
	{
		heroDirection = Beside;
	}
	else
	{
		heroDirection = NotFound;
	}

	if (heroDirection == NotFound)
	{
		state = CLive;
	}
	else
	{
		state = CAlert;
	}
}

int Creature::damage()
{
	return attack;
}

int Creature::getX()
{
	return x;
}

int Creature::getY()
{
	return y;
}

int Creature::hurt(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		state = CDeath;
		return exp;
	}
	return 0;
}

int Creature::getState()
{
	return state;
}

int Creature::getHealth()
{
	return health;
}

void Creature::move(int _x, int _y)
{
	if (energy >= 1)
	{
		x += _x;
		y += _y;
		energy--;
	}
	else
	{
		energy++;
	}
}

int Creature::getEnergy()
{
	return energy;
}

int Creature::getHeroDirection()
{
	return heroDirection;
}

bool Creature::isLive()
{
	return state != CDeath;
}