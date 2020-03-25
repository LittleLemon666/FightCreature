#pragma once
enum heroState
{
	HLive,
	HInvincible,
	HDeath
};

enum SwordDirection
{
	SNorth,
	SEast,
	SWest,
	SSouth
};

class Hero
{
private:
	int x = 1;
	int y = 1;
	int health = 10;
	int attack = 3;
	int state = HLive;
	int swordDirection = SSouth;
	int experience = 0;
	int level = 1;

public:
	void setHeroLocation(int dungeonWidth, int dungeonHeight, int mode);
	int setHeroLocation(std::vector<std::string> lineString, int lineIndex);
	int getX();
	int getY();
	int getState();
	int getSwordDirection();
	int getHealth();
	int getExperience();
	int getLevel();
	bool touchCreature(int creatureX, int creatureY);
	void hurt(int damage);
	void move(int _x, int _y);
	int slash(int creatureX, int creatureY);
	bool isLive();
	void getExp(int exp);
	void information();
};