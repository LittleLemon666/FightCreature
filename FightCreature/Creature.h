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
	NotFound
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
	int heroDirection = NotFound;
	int energy = 1;
	int exp = 1;

public:
	void setCreatureLocation(int dungeonWidth, int dungeonHeight, int mode);
	void setCreatureLocation(int _x, int _y);
	int setCreatureInformation(std::vector<std::string>& stringLine, int lineIndex);
	void seeHero(int heroX, int heroY);
	int damage();
	int getX();
	int getY();
	int hurt(int damage);
	int getState();
	int getHealth();
	void move(int _x, int _y);
	int getEnergy();
	int getHeroDirection();
	bool isLive();
};