#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include "Hero.h"
using namespace std;

void Hero::setHeroLocation(int dungeonWidth, int dungeonHeight, int mode) //mode 1:QuickGame 2:LoadGame 3:CustomGame
{
	switch(mode)
	{
	case 1:
		x = rand() % (dungeonWidth - 2) + 1;
		y = rand() % (dungeonHeight - 2) + 1;
		break;
	case 2:
		break;
	case 3:
		cout << "\n";
		cout << "Set hero's location\n";
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

int Hero::setHeroLocation(vector<string> lineString, int lineIndex)
{
	stringstream ss;
	ss << lineString[lineIndex++];
	ss >> x >> y >> health >> attack;
	return lineIndex;
}

int Hero::getSwordDirection()
{
	return swordDirection;
}

int Hero::getX()
{
	return x;
}

int Hero::getY()
{
	return y;
}

int Hero::getState()
{
	return state;
}

int Hero::getHealth()
{
	return health;
}

int Hero::getExperience()
{
	return experience;
}

int Hero::getLevel()
{
	return level;
}

bool Hero::touchCreature(int creatureX, int creatureY)
{
	return x == creatureX && y == creatureY;
}

clock_t Hero::hurt(int damage)
{
	if (state == HInvincible)
	{
		return InvincibleBegin;
	}
	health -= damage;
	if (health <= 0)
	{
		state = HDeath;
	}
	else
	{
		state = HInvincible;
	}
	return InvincibleBegin = clock();
}

void Hero::vincible()
{
	state = HLive;
}

void Hero::move(int _x, int _y)
{
	if (_x == -1)
	{
		swordDirection = SWest;
	}
	else if (_x == 1)
	{
		swordDirection = SEast;
	}
	else if (_y == -1)
	{
		swordDirection = SNorth;
	}
	else if (_y == 1)
	{
		swordDirection = SSouth;
	}
	x += _x;
	y += _y;
}

int Hero::slash(int creatureX, int creatureY)
{
	if (creatureX == x && creatureY == y)
	{
		return attack;
	}
	else if (swordDirection == SNorth)
	{
		if (creatureX == x && y - creatureY == 1)
		{
			return attack;
		}
	}
	else if (swordDirection == SEast)
	{
		if (creatureX - x == 1 && y == creatureY)
		{
			return attack;
		}
	}
	else if (swordDirection == SWest)
	{
		if (x - creatureX == 1 && y == creatureY)
		{
			return attack;
		}
	}
	else if (swordDirection == SSouth)
	{
		if (creatureX == x && creatureY - y == 1)
		{
			return attack;
		}
	}
	return 0;
}

bool Hero::isLive()
{
	return state != HDeath;
}

void Hero::getExp(int exp)
{
	experience += exp;
	while (experience >= level * (level - 1) + level)
	{
		experience -= level * (level - 1) + level;
		level++;
	}
}

void Hero::information()
{
	cout << "Hero health:" << setw(health + (streamsize)1) << setfill('#') << " " << health << " ";
	cout << "Exp: " << experience << " Level: " << level;
	cout << " Hero is facing ";
	switch (swordDirection)
	{
	case SNorth:
		cout << "North";
		break;
	case SSouth:
		cout << "South";
		break;
	case SEast:
		cout << "East";
		break;
	case SWest:
		cout << "West";
		break;
	}
	cout << "\n";
}