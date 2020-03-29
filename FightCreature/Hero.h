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
	int maxHealth = 10;
	int attack = 3;
	int state = HLive;
	int swordDirection = SSouth;
	int experience = 0;
	int level = 1;
	clock_t InvincibleBegin = clock();
	char skin = 'H';

public:
	Hero();
	~Hero();
	bool isLive();
	int getX();
	int getY();
	int getSwordDirection();
	void vincible();
	void setHeroLocation(int dungeonWidth, int dungeonHeight, int mode);
	int loadHeroInformation(int _x, int _y, std::vector<std::string> lineString, int lineIndex, char heroSkin);
	void move(int _x, int _y);
	bool touchCreature(int creatureX, int creatureY);
	clock_t hurt(int damage);
	int slash(int creatureX, int creatureY);
	void getExp(int exp);
	std::string information();
	char getSkin();
};