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

const Point Hero::getPosition() const
{
	return Point(x, y);
}

const int Hero::getSwordDirection() const
{
	return swordDirection;
}

const void Hero::vincible()
{
	if (clock() - InvincibleStart > defaultHeroInvincibleTime)
	{
		state = HLive;
	}
}

const void Hero::setHeroLocation(const Point dungeonRectangle, int&& mode) //mode 1:QuickGame 2:LoadGame 3:CustomGame
{
	switch (mode)
	{
	case 1:
		x = rand() % (dungeonRectangle.X - 2) + 1;
		y = rand() % (dungeonRectangle.Y - 2) + 1;
		break;
	case 3:
		cout << "\n";
		cout << "Set hero's location\n";
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

const void Hero::move(const Point dxy)
{
	if (dxy.X == -1)
	{
		swordDirection = SWest;
	}
	else if (dxy.X == 1)
	{
		swordDirection = SEast;
	}
	else if (dxy.Y == -1)
	{
		swordDirection = SNorth;
	}
	else if (dxy.Y == 1)
	{
		swordDirection = SSouth;
	}
	x += dxy.X;
	y += dxy.Y;
}

const bool Hero::touchCreature(const Point creaturePosition)
{
	return x == creaturePosition.X && y == creaturePosition.Y;
}

const void Hero::hurt(const int damage)
{
	if (state == HInvincible)
	{
		InvincibleStart = InvincibleBegin;
		vincible();
		return;
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
	InvincibleStart = InvincibleBegin = clock();
}

const int Hero::slash(const Point creaturePosition) const
{
	if (creaturePosition.X == x && creaturePosition.Y == y)
	{
		return attack;
	}
	else if (swordDirection == SNorth)
	{
		if (creaturePosition.X == x && y - creaturePosition.Y == 1)
		{
			return attack;
		}
	}
	else if (swordDirection == SEast)
	{
		if (creaturePosition.X - x == 1 && y == creaturePosition.Y)
		{
			return attack;
		}
	}
	else if (swordDirection == SWest)
	{
		if (x - creaturePosition.X == 1 && y == creaturePosition.Y)
		{
			return attack;
		}
	}
	else if (swordDirection == SSouth)
	{
		if (creaturePosition.X == x && creaturePosition.Y - y == 1)
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