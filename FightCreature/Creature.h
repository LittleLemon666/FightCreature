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
	int maxHealth = 5;
	int attack = 1;
	int state = CLive;
	int heroDirection = NotFound;
	int energy = 1;
	int maxEnergy = 1;
	int exp = 1;

public:
	Creature();
	~Creature();
	bool isLive();
	bool isAlert();
	int getX();
	int getY();
	int damage();
	void setCreatureLocation(int dungeonWidth, int dungeonHeight, int mode);
	void loadCreatureLocation(int _x, int _y);
	int loadCreatureInformation(std::vector<std::string>& stringLine, int lineIndex);
	int hurt(int damage);
	void seeHero(int heroX, int heroY);
	void move(int _x, int _y);
	void information();
};