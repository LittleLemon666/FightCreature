#pragma once
#include "Point.h"
#include "Creature.h"
#include "Trigger.h"
#include "Trap.h"
using namespace std;

class Dungeon
{
private:
	int width = 28;
	int height = 14;
	char wall = '#';
	char floor = ' ';
	vector<string> dungeonMap;

public:
	Dungeon();
	~Dungeon();
	const int getWidth() const;
	const int getHeight() const;
	const char getFloorSkin() const;
	const bool isObstacle(const Point position) const;
	const bool isBoundary(const Point position) const;
	const void fill();
	const void generatePlain();
	const int loadMap(vector<string> lineString);
	const void customMap();
	const void generateTerrain(const Point heroPosition, vector<Point> creaturePoints, const int creatureNum);
	const void generateTerrain(const Point heroPosition, const Point creaturePosition);
	const bool searchPath(Point nowPosition, Point targetPosition, int p, int pStop);
	vector<string> outputMap() const;
	vector<Creature> creatures;
	vector<char> creaturesSkinList;
	vector<Trigger> triggers;
	vector<Trap> traps;
	static int dungeonIndex;
};