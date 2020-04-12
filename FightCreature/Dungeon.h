#pragma once
#include "Point.h"

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
	const bool isObstacle(int x, int y) const;
	const bool isBoundary(int x, int y) const;
	const void fill();
	const void generatePlain();
	const int loadMap(vector<string> lineString);
	const void customMap();
	const void generateTerrain(const int heroX, const int heroY, vector<Point> creaturePoints, const int creatureNum);
	const void generateTerrain(const int heroX, const int heroY, const int creatureX, const int creatureY);
	const bool searchPath(int nowX, int nowY, int targetX, int targetY, int p, int pStop);
	vector<string> outputMap() const;
};