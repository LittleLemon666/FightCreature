#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include "Hero.h"
using namespace std;

Hero::Hero()
{

}

Hero::~Hero()
{

}

bool Hero::isLive()
{
	return state != HDeath;
}

int Hero::getX()
{
	return x;
}

int Hero::getY()
{
	return y;
}

int Hero::getSwordDirection()
{
	return swordDirection;
}

void Hero::vincible()
{
	state = HLive;
}

void Hero::setHeroLocation(int dungeonWidth, int dungeonHeight, int mode) //mode 1:QuickGame 2:LoadGame 3:CustomGame
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

int Hero::loadHeroInformation(int _x, int _y, vector<string> lineString, int lineIndex, char heroSkin)
{
	x = _x;
	y = _y;
	skin = heroSkin;
	stringstream ss;
	ss << lineString[lineIndex++];
	ss >> maxHealth >> attack >> experience >> level;
	health = maxHealth;
	return lineIndex;
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

void Hero::getExp(int exp)
{
	experience += exp;
	int nextExperience = (level - 2) * (level - 1) + level;
	while (experience >= nextExperience)
	{
		experience -= nextExperience;
		level++;
		if (level % 3 == 0)
		{
			attack++;
		}
	}
}

string Hero::information()
{
	stringstream ss;
	ss << "Hero(" << skin << ")'s health:";
	if (health > 0)
	{
		ss << setw(health) << setfill('#') << '#';
	}
	ss << setw((streamsize)(maxHealth - health + 1)) << setfill(' ') << ' ';
	ss << setw(2) << right << health << " ";
	ss << "Exp: " << experience << " Level: " << level << " attack: " << attack;
	ss << " Hero is facing ";
	switch (swordDirection)
	{
	case SNorth:
		ss << "North";
		break;
	case SSouth:
		ss << "South";
		break;
	case SEast:
		ss << "East";
		break;
	case SWest:
		ss << "West";
		break;
	}
	return ss.str();
}

char Hero::getSkin()
{
	return skin;
}