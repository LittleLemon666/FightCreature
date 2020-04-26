#pragma once
#include <iostream>
#include <iomanip> //clock_t
#include <vector>
#include <sstream>
#include "Point.h"
using namespace std;

enum heroState
{
	HLive,
	HInvincible,
	HDeath
};

enum SwordDirection
{
	SNorth,
	SSouth,
	SWest,
	SEast
};

class Hero
{
private:
	int x = 1;
	int y = 1;
	int health = 10;
	int maxHealth = 10;
	int attack = 3;
	int state = HLive;
	int swordDirection = SSouth;
	int experience = 0;
	int level = 1;
	clock_t InvincibleStart;
	clock_t InvincibleBegin = clock();
	clock_t InvincibleEnd;
	static const int defaultHeroInvincibleTime = 700;
	char skin = 'H';

public:
	Hero();
	~Hero();
	const bool isLive() const;
	const int getX() const;
	const int getY() const;
	const Point getPosition() const;
	const int getSwordDirection() const;
	const void vincible();
	const void setHeroLocation(const Point dungeonRectangle, int&& mode);
	const int loadHeroInformation(const int _x, const int _y, vector<string> lineString, int lineIndex, const char heroSkin);
	const void move(const Point dxy);
	const bool touchCreature(const Point creaturePosition);
	const void hurt(const int damage);
	const int slash(const Point creaturePosition) const;
	const void getExp(const int exp);
	const string information() const;
	const char getSkin() const;

};