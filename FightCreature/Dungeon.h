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
	int getWidth() const;
	int getHeight() const;
	char getFloorSkin() const;
	bool isObstacle(int x, int y) const;
	bool isBoundary(int x, int y) const;
	void fill();
	void generatePlain();
	int loadMap(std::vector<std::string> lineString);
	void customMap();
	void generateTerrain(const int heroX, const int heroY, std::vector<Point> creaturePoints, const int creatureNum);
	void generateTerrain(const int heroX, const int heroY, const int creatureX, const int creatureY);
	bool searchPath(int nowX, int nowY, int targetX, int targetY, int p, int pStop);
	std::vector<std::string> outputMap() const;
};