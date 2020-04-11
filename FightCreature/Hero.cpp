#include <iostream>
#include <iomanip> //clock_t
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

const bool Hero::isLive() const
{
	return state != HDeath;
}

const int Hero::getX() const
{
	return x;
}

const int Hero::getY() const
{
	return y;
}

const int Hero::getSwordDirection() const
{
	return swordDirection;
}

const void Hero::vincible()
{
	state = HLive;
}

const void Hero::setHeroLocation(const int dungeonWidth, const int dungeonHeight, int&& mode) //mode 1:QuickGame 2:LoadGame 3:CustomGame
{
	switch (mode)
	{
	case 1:
		x = rand() % (dungeonWidth - 2) + 1;
		y = rand() % (dungeonHeight - 2) + 1;
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

const int Hero::loadHeroInformation(const int _x, const int _y, vector<string> lineString, int lineIndex, const char heroSkin)
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

const void Hero::move(const int _x, const int _y)
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

const bool Hero::touchCreature(const int creatureX, const int creatureY)
{
	return x == creatureX && y == creatureY;
}

const clock_t Hero::hurt(const int damage)
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

const int Hero::slash(const int creatureX, const int creatureY) const
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

const void Hero::getExp(const int exp)
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

const string Hero::information() const
{
	string heroInformation = "";
	heroInformation += "Hero(" + string(1, skin) + ")'s health:";
	if (health > 0)
	{
		heroInformation += string(health, '#');
		heroInformation += string(maxHealth - health + 1, ' ');
	}
	else
	{
		heroInformation += string(maxHealth + 1, ' ');
	}
	heroInformation += to_string(health);
	heroInformation += " Exp: " + to_string(experience) + " Level: " + to_string(level) + " attack: " + to_string(attack);
	heroInformation += " Hero is facing ";
	switch (swordDirection)
	{
	case SNorth:
		heroInformation += "North";
		break;
	case SSouth:
		heroInformation += "South";
		break;
	case SEast:
		heroInformation += "East";
		break;
	case SWest:
		heroInformation += "West";
		break;
	}
	return heroInformation;
}

const char Hero::getSkin() const
{
	return skin;
}