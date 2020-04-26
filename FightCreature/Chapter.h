#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <windows.h>    // LPWSTR ConvertString  cvtLPW2stdstring  read_directory
#include <Mmsystem.h>   //mp3
#include <mciapi.h>     //mp3
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib") //mp3
#include "Direction.h"
#include "Dungeon.h"
#include "Hero.h"
#include "Arrow.h"
#include "TextColorChange.h"
#include "LoadFiles.h"
#include "KeyState.h"
using namespace std;

enum GameMode
{
	QuickGame = 1,
	LoadGame = 2,
	CustomGame = 3
};

enum GameState
{
	PREPARING,
	GAMING,
	GAMEOVER
};

class Chapter
{
private:
	vector<string> screen;
	vector<Arrow> arrows;
	const int defaultcreatureNum = 3;
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	static int gameState;
	vector<Dungeon> dungeons;
	Hero hero;
	static bool isMountDemonSlayer;

public:
	Chapter();
	~Chapter();
	const void gameInformation();
	const void creatureInformation();
	const void update(bool key[]);
	//game mode
	const void quickGame();
	const int loadGame(bool key[]);
	const void customGame();
	const void generateTerrainAndRegenerateTerrain();
	const void gaming(bool key[]);
	//load FCF
	const void preview(const string mapPath);
	const void loadMapInformation(const string mapPath);
	const bool loadFindHeroLocation(int& heroX, string& stringT, const char floor, const char heroSkin);
	const bool loadFindCreatureLocation(vector<Creature>& creature, const int creatureY, string& stringT, const char floor, vector<char>& creaturesSkinList, vector<int>& creaturesPropertyIndex);
	const bool loadFindTrapLocation(vector<Trap>& traps, const int trapY, string& stringT, const char floor, const char trapSkin);
	const void heroSwordAttack();
	const void heroBowAttack();
	const void heroMove(const int _x, const int _y);
	const void creaturesTurn();
	const void trackHero(const int heroX, const int heroY, const int creatureX, const int creatureY, Creature& creatureT);
	const void heroBeDamaged();
	const void generateTrigger();
	const void arrowsTurn();
	const void objectAction();
	const void draw();
	const void printScreen();
	const void isGameOver(bool key[]);
	//DemonSlayer
	const bool DemonSlayer_isMount() const;
	const void DemonSlayer_BreathOfThunder();
	const void DemonSlayer_BreathOfWater1();
	const void DemonSlayer_BreathOfWater11();

};