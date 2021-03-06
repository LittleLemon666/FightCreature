#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Point.h"
enum Direction
{
	North,
	South,
	East,
	West,
	NorthEast,
	NorthWest,
	SouthWest,
	SouthEast,
	Beside,
	NotFound
};

enum CreatureState
{
	CLive,
	CAlert,
	CDeath
};

using namespace std;
class Creature
{
private:
	int x = 1;
	int y = 1;
	int health = 5;
	int maxHealth = 5;
	int attack = 1;
	int state = CLive;
	int heroDirection = NotFound;
	int energy = 1;
	int maxEnergy = 1;
	int exp = 1;
	char skin = 'C';
	static const int defaultCreatureSpeed = 300;

public:
	Creature();
	~Creature();
	static int creatureTotal;
	static int creatureNum;
	const bool isLive() const;
	const bool isAlert() const;
	const int getX() const;
	const int getY() const;
	const Point getPosition() const;
	const int damage() const;
	const void setCreatureLocation(const Point dungeonRectangle, const int mode);
	const void loadCreatureLocation(const Point creaturePosition);
	const int loadCreatureInformation(vector<string>& stringLine, const int lineIndex, vector<char>& creaturesSkineList, const int creaturesPropertyIndex);
	const int hurt(const int damage);
	static const bool canCreaturesTurn();
	const void seeHero(const Point heroPosition);
	const void move(const Point dxy);
	const void energyRecovery();
	const string information() const;
	const char getSkin() const;
	static clock_t creatureTimeBegin;

};