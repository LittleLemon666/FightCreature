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
	char skin = 'C';

public:
	Creature();
	~Creature();
	static int creatureTotal;
	static int creatureNum;
	bool isLive() const;
	bool isAlert() const;
	int getX() const;
	int getY() const;
	int damage() const;
	void setCreatureLocation(const int dungeonWidth, const int dungeonHeight, const int mode);
	void loadCreatureLocation(const int _x, const int _y);
	int loadCreatureInformation(std::vector<std::string>& stringLine, const int lineIndex, std::vector<char>& creaturesSkineList, const int creaturesPropertyIndex);
	int hurt(const int damage);
	void seeHero(const int heroX, const int heroY);
	void move(const int _x, const int _y);
	void energyRecovery();
	std::string information() const;
	char getSkin() const;
};