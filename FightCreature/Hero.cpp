#include <iostream>
#include "Hero.h"

void Hero::setHeroLocation(int dungeonWidth, int dungeonHeight, int mode) //mode 1:QuickGame 2:LoadGame 3:CustomGame
{
	switch(mode)
	{
	case 1:
		x = rand() % (dungeonWidth - 1) + 1;
		y = rand() % (dungeonHeight - 1) + 1;
		break;
	case 2:
		break;
	case 3:
		std::cout << "\n";
		std::cout << "Set hero's location\n";
		bool valid;
		do
		{
			valid = true;
			std::cout << "x (1 ~ " << dungeonWidth - 2 << "): ";
			std::cin >> x;
			if (x < 1 || x >= dungeonWidth)
			{
				valid = false;
				std::cout << "input is out of range (1 ~ " << dungeonWidth - 2 << ")\n";
			}
		} while (!valid);

		do
		{
			valid = true;
			std::cout << "y (1 ~ " << dungeonHeight - 2 << "): ";
			std::cin >> y;
			if (y < 1 || y >= dungeonHeight)
			{
				valid = false;
				std::cout << "input is out of range (1 ~ " << dungeonHeight - 2 << ")\n";
			}
		} while (!valid);
		break;
	}
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

bool Hero::touchCreature(int creatureX, int creatureY)
{
	return x == creatureX && y == creatureY;
}

void Hero::hurt(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		state = HDeath;
	}
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