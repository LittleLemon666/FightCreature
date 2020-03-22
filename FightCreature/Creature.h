#pragma once
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
	Unknown
};

enum CreatureState
{
	CLive,
	CAlert,
	CDeath
};

class Creature
{
private:
	int x = 1;
	int y = 1;
	int health = 5;
	int attack = 1;
	int state = CLive;
	int heroDirection = Unknown;
	int energy = 1;

public:
	void setCreatureLocation(int dungeonWidth, int dungeonHeight, int mode);
	int setCreatureLocation(std::vector<std::string> lineString, int lineIndex);
	void seeHero(int heroX, int heroY);
	int damage();
	int getX();
	int getY();
	void hurt(int damage);
	int getState();
	int getHealth();
	void move(int _x, int _y);
	int getEnergy();
	int getHeroDirection();
	bool isLive();
};