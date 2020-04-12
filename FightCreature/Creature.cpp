#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include "Creature.h"
using namespace std;

int Creature::creatureTotal = 0;
int Creature::creatureNum = 0;
clock_t Creature::creatureTimeBegin = clock();
Creature::Creature()
{

}

Creature::~Creature()
{

}

const bool Creature::isLive() const
{
	return state != CDeath;
}

const bool Creature::isAlert() const
{
	return state == CAlert;
}

const int Creature::getX() const
{
	return x;
}

const int Creature::getY() const
{
	return y;
}

const int Creature::damage() const
{
	return attack;
}

const void Creature::setCreatureLocation(const int dungeonWidth, const int dungeonHeight, const int mode)
{
	switch (mode)
	{
	case 1:
		x = rand() % (dungeonWidth - 2) + 1;
		y = rand() % (dungeonHeight - 2) + 1;
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

const void Creature::loadCreatureLocation(const int _x, const int _y)
{
	x = _x;
	y = _y;
}

const int Creature::loadCreatureInformation(vector<string>& stringLine, const int lineIndex, vector<char>& creaturesSkineList, const int creaturesPropertyIndex)
{
	stringstream ss;
	ss << stringLine[lineIndex + creaturesPropertyIndex];
	ss >> health >> attack >> energy >> exp;
	skin = creaturesSkineList[creaturesPropertyIndex];
	maxHealth = health;
	maxEnergy = energy;
	ss.str("");
	ss.clear();
	return lineIndex;
}

const int Creature::hurt(const int damage)
{
	health -= damage;
	if (health <= 0)
	{
		state = CDeath;
		return exp;
	}
	return 0;
}

const bool Creature::canCreaturesTurn()
{
	if (clock() - creatureTimeBegin > defaultCreatureSpeed)
	{
		creatureTimeBegin = clock();
		return true;
	}
	return false;
}

const void Creature::seeHero(const int heroX, const int heroY)
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

const void Creature::move(const int _x, const int _y)
{
	if (energy >= maxEnergy)
	{
		x += _x;
		y += _y;
		energy = 0;
	}
	else
	{
		energyRecovery();
	}
}

const void Creature::energyRecovery()
{
	energy++;
	if (energy > maxEnergy)
	{
		energy = maxEnergy;
	}
}

const string Creature::information() const
{
	string creatureInformation = "";
	creatureInformation += "Creature(" + string(1, skin) + ")'s health:";
	if (health > 0)
	{
		creatureInformation += string(health, '#');
		creatureInformation += string(maxHealth - health + 1, ' ');
	}
	else
	{
		creatureInformation += string(maxHealth + 1, ' ');
	}
	creatureInformation += to_string(health);
	creatureInformation += " Energy:" + string(energy, '#') + string(maxEnergy - energy + 1, ' ') + to_string(energy);
	creatureInformation += " attack: " + to_string(attack) + " ";
	if (state == CAlert)
	{
		if (heroDirection == Beside)
		{
			creatureInformation += "The creature is beside you.";
		}
		else
		{
			creatureInformation += "Hero is to the ";
			switch (heroDirection)
			{
			case East:
				creatureInformation += "east";
				break;
			case West:
				creatureInformation += "west";
				break;
			case North:
				creatureInformation += "north";
				break;
			case South:
				creatureInformation += "south";
				break;
			case NorthEast:
				creatureInformation += "northeast";
				break;
			case SouthEast:
				creatureInformation += "southeast";
				break;
			case NorthWest:
				creatureInformation += "northwest";
				break;
			case SouthWest:
				creatureInformation += "southwest";
				break;
			}
		}
	}
	return creatureInformation;
}

const char Creature::getSkin() const
{
	return skin;
}