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

const Point Creature::getPosition() const
{
	return Point(x, y);
}

const int Creature::damage() const
{
	return attack;
}

const void Creature::setCreatureLocation(const Point dungeonRectangle, const int mode)
{
	switch (mode)
	{
	case 1:
		x = rand() % (dungeonRectangle.X - 2) + 1;
		y = rand() % (dungeonRectangle.Y - 2) + 1;
		break;
	case 3:
		cout << "\n";
		cout << "Set creature's location\n";
		bool valid;
		do
		{
			valid = true;
			cout << "x (1 ~ " << dungeonRectangle.X - 2 << "): ";
			cin >> x;
			if (x < 1 || x >= dungeonRectangle.X - 1)
			{
				valid = false;
				cout << "input is out of range (1 ~ " << dungeonRectangle.X - 2 << ")\n";
			}
		} while (!valid);

		do
		{
			valid = true;
			cout << "y (1 ~ " << dungeonRectangle.Y - 2 << "): ";
			cin >> y;
			if (y < 1 || y >= dungeonRectangle.Y - 1)
			{
				valid = false;
				cout << "input is out of range (1 ~ " << dungeonRectangle.Y - 2 << ")\n";
			}
		} while (!valid);
		break;
	}
}

const void Creature::loadCreatureLocation(const Point creaturePosition)
{
	x = creaturePosition.X;
	y = creaturePosition.Y;
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

const void Creature::seeHero(const Point heroPosition)
{
	if (x > heroPosition.X && x - heroPosition.X <= 3 && y == heroPosition.Y)
	{
		heroDirection = West;
	}
	else if (x < heroPosition.X && x - heroPosition.X >= -3 && y == heroPosition.Y)
	{
		heroDirection = East;
	}
	else if (x > heroPosition.X && x - heroPosition.X <= 3 && y > heroPosition.Y && y - heroPosition.Y <= 3)
	{
		heroDirection = NorthWest;
	}
	else if (x > heroPosition.X && x - heroPosition.X <= 3 && y < heroPosition.Y && y - heroPosition.Y >= -3)
	{
		heroDirection = SouthWest;
	}
	else if (x < heroPosition.X && x - heroPosition.X >= -3 && y > heroPosition.Y && y - heroPosition.Y <= 3)
	{
		heroDirection = NorthEast;
	}
	else if (x < heroPosition.X && x - heroPosition.X >= -3 && y < heroPosition.Y && y - heroPosition.Y >= -3)
	{
		heroDirection = SouthEast;
	}
	else if (x == heroPosition.X && y < heroPosition.Y && y - heroPosition.Y >= -3)
	{
		heroDirection = South;
	}
	else if (x == heroPosition.X && y > heroPosition.Y && y - heroPosition.Y <= 3)
	{
		heroDirection = North;
	}
	else if (x == heroPosition.X && y == heroPosition.Y)
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

const void Creature::move(const Point dxy)
{
	if (energy >= maxEnergy)
	{
		x += dxy.X;
		y += dxy.Y;
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