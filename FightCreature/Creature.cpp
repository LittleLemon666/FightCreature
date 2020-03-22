#include <iostream>
#include "Creature.h"

void Creature::setCreatureLocation(int dungeonWidth, int dungeonHeight)
{
	std::cout << "\n";
	std::cout << "Set creature's location\n";
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
		heroDirection = Unknown;
	}

	if (heroDirection == Unknown)
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

void Creature::hurt(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		state = CDeath;
	}
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