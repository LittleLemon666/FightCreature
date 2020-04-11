#pragma once
#include "Point.h"

class Dungeon
{
private:
	int width = 28;
	int height = 14;
	char wall = '#';
	char floor = ' ';
	std::vector<std::string> dungeonMap;

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
	const int loadMap(std::vector<std::string> lineString);
	const void customMap();
	const void generateTerrain(const int heroX, const int heroY, std::vector<Point> creaturePoints, const int creatureNum);
	const void generateTerrain(const int heroX, const int heroY, const int creatureX, const int creatureY);
	const bool searchPath(int nowX, int nowY, int targetX, int targetY, int p, int pStop);
	std::vector<std::string> outputMap() const;
};