#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <conio.h>		// _getch() <= to read input without enter "ENTER" key
#include <windows.h>    // LPWSTR ConvertString  cvtLPW2stdstring  read_directory
#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")
#include "Dungeon.h"
#include "Hero.h"
#include "Creature.h"
#include "Trigger.h"
#include "Point.h"
#include "LoadFiles.h"
#define defaultCreatureNum 5
#define defaultCreatureSpeed 300
#define defaultHeroInvincibleTime 700
#define defaultTriggerGenerateTimeFrame 2000
using namespace std;
using std::ifstream;

int dx[] = { 0, 0, -1, 1 };
int dy[] = { -1, 1, 0, 0 };

Dungeon dungeon;
Hero hero;
vector<Creature> creatures;
vector<char> creaturesSkinList;
vector<Trigger> triggers;
static int creatureTotal;
static int creatureNum;
static int triggerTotal;
static int gameState;
static clock_t InvincibleEnd;
static clock_t InvincibleStart;
static bool isMountDemonSlayer;
vector<string> screen;

enum KeyState
{
    ESC,
    W,
    S,
    A,
    D,
    SPACE,
    UA,
    DA,
    LA,
    RA,
    ENTER,
    ZERO,
    ONE,
    TWO,
    INVALID
};

enum GameState
{
    PREPARING,
    GAMING,
    GAMEOVER
};

enum GameMode
{
    QuickGame = 1,
    LoadGame = 2,
    CustomGame = 3
};

class DemonSlayer
{
public:
    bool isMount();
    void BreathOfThunder();
    void BreathOfWater1();
    void BreathOfWater11();
};

DemonSlayer demonSlayer;

HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
void maximumWindows();
void fontsize(int, int);
void getKey(bool key[]);
void update(bool key[]);
bool menu();
void gameInformation();
//game mode
void quickGame();
int loadGame();
void customGame();
//load FCF
void preview(string mapPath);
void loadMapInformation(string mapPath);
bool loadFindHeroLocation(int& heroX, string& stringT, char floor, char heroSkin);
bool loadFindCreatureLocation(vector<Creature>& creature, int creatureY, string& stringT, char floor, vector<char>& creaturesSkinList, vector<int>& creaturesPropertyIndex);
void generateTerrainAndRegenerateTerrain();
//gaming
void creaturesTurn();
void trackHero(int heroX, int heroY, int creatureX, int creatureY, Creature& creatureT);
void heroBeDamaged();
void creatureInformation();
void generateTrigger();
void draw();
void printScreen();
void isGameOver();

int main()
{
    isMountDemonSlayer = demonSlayer.isMount(); //extension
    srand(time(NULL));
    maximumWindows();
    if (!menu())
    {
        return 0;
    }
    gameState = PREPARING;
    triggerTotal = 0;

    bool key[INVALID + 1] = { false };

    update(key);
    gameState = GAMING;
    clock_t creatureTimeBegin = clock();
    clock_t creatureTimeEnd;
    clock_t triggerTimeBegin = clock();
    clock_t triggerTimeEnd;
    do
    {
        if (_kbhit())
        {
            getKey(key);
            update(key);
        }
        if (gameState == GAMEOVER)
        {
            break;
        }
        creatureTimeEnd = clock();
        if (creatureTimeEnd - creatureTimeBegin > defaultCreatureSpeed)
        {
            creaturesTurn();
            creatureTimeBegin = clock();
        }
        triggerTimeEnd = clock();
        if (triggerTimeEnd - triggerTimeBegin > defaultTriggerGenerateTimeFrame)
        {
            generateTrigger();
            triggerTimeBegin = clock();
        }
        heroBeDamaged();
        draw();
        isGameOver();
    }while (!key[ESC] && gameState != GAMEOVER);

    return 0;
}

void maximumWindows()
{
    //Author: ashishchoure
    //URL: https://www.daniweb.com/programming/software-development/threads/308396/setting-the-console-window-size
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1920, 1080, TRUE);
}

void fontsize(int a, int b)
{
    //URL: http://mycodecollection.blogspot.com/2015/01/c-console-change-font-size.html
    PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
    lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
    lpConsoleCurrentFontEx->dwFontSize.X = a;
    lpConsoleCurrentFontEx->dwFontSize.Y = b;
    SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}

void getKey(bool key[])
{
    for (int i = 0; i < INVALID; i++)
    {
        key[i] = false;
    }
    char inputKey = _getch();
    switch (inputKey)
    {
    case 'w':
    case 'W':
        key[W] = true;
        break;
    case 's':
    case 'S':
        key[S] = true;
        break;
    case 'a':
    case 'A':
        key[A] = true;
        break;
    case 'd':
    case 'D':
        key[D] = true;
        break;
    case ' ':
        key[SPACE] = true;
        break;
    case 27:
        key[ESC] = true;
        break;
    case 13:
        key[ENTER] = true;
        break;
    case 72:
        key[UA] = true;
        break;
    case 80:
        key[DA] = true;
        break;
    case 75:
        key[LA] = true;
        break;
    case 77:
        key[RA] = true;
        break;
    case '0':
        key[ZERO] = true;
        break;
    case '1':
        key[ONE] = true;
        break;
    case '2':
        key[TWO] = true;
        break;
    }
}

void update(bool key[])
{
    if (gameState == GAMING)
    {
        int _x = 0, _y = 0;
        if (key[W] || key[UA])
        {
            _x = 0;
            _y = -1;
        }
        else if (key[S] || key[DA])
        {
            _x = 0;
            _y = 1;
        }
        else if (key[A] || key[LA])
        {
            _x = -1;
            _y = 0;
        }
        else if (key[D] || key[RA])
        {
            _x = 1;
            _y = 0;
        }
        else if (key[SPACE])
        {
            for (int i = 0; i < creatureTotal; i++)
            {
                if (creatures[i].isLive())
                {
                    hero.getExp(creatures[i].hurt(hero.slash(creatures[i].getX(), creatures[i].getY())));
                }
            }
        }
        else if (isMountDemonSlayer && key[ZERO])
        {
            demonSlayer.BreathOfWater11();
        }
        else if (isMountDemonSlayer && key[ONE])
        {
            demonSlayer.BreathOfThunder();
        }
        else if (isMountDemonSlayer && key[TWO])
        {
            demonSlayer.BreathOfWater1();
        }
        else
        {
            return;
        }
        isGameOver();
        if (gameState == GAMEOVER)
        {
            return;
        }

        if (!dungeon.isBoundary(hero.getX() + _x, hero.getY() + _y) && !dungeon.isObstacle(hero.getX() + _x, hero.getY() + _y))
        {
            hero.move(_x, _y);
            for (int i = 0; i < triggerTotal; i++)
            {
                if (triggers[i].isExist() && triggers[i].getX() == hero.getX() && triggers[i].getY() == hero.getY())
                {
                    hero.getExp(triggers[i].getTrigger());
                }
            }
        }
    }
}

bool menu()
{
    cout << "Quick game please press 1.\n";
    cout << "Load game please press 2.\n";
    cout << "Custom game please press 3.\n";
    cout << "Press ESC to leave.\n";
    int result;
    while (true)
    {
        char choose = _getch();

        switch (choose)
        {
        case '1':
            quickGame();
            return true;

        case '2':
            result = loadGame();
            if (result == 0)
            {
                menu();
            }
            return true;

        case '3':
            customGame();
            return true;

        case 27: //ESC
            return false;
        }
    }
}

void gameInformation()
{
    cout << "Press WASD or arrow keys to control Hero(icon " << hero.getSkin() << ").\n";
    cout << "Press Space to attack in the direction where Hero is facing.\n";
    cout << "Avoid touching enemies (icon ";
    for (int i = 0; i < creaturesSkinList.size(); i++)
    {
        cout << creaturesSkinList[i];
    }
    cout << ") or Hero will take damage.\n";
    cout << "Pick up triggers(+) will get some exp.\n";
    cout << "Use Hero's sword to knock out creatures!\n";
    cout << "Press ESC to exit the game.\n";
}

void quickGame()
{
    vector<Point> point;
    dungeon.generatePlain();
    dungeon.printMap();
    hero.setHeroLocation(dungeon.getWidth(), dungeon.getHeight(), QuickGame);
    creatureTotal = defaultCreatureNum;
    creatureNum = creatureTotal;
    for (int i = 0; i < creatureTotal; i++)
    {
        creatures.push_back(Creature());
        creatures[i].setCreatureLocation(dungeon.getWidth(), dungeon.getHeight(), QuickGame);
        point.push_back(Point(creatures[i].getX(), creatures[i].getY()));
    }
    dungeon.generateTerrain(hero.getX(), hero.getY(), point, creatureTotal);
    draw();
    generateTerrainAndRegenerateTerrain();
}

int loadGame()
{
    vector<string> filePathes;
    LoadFiles loadFiles;
    loadFiles.read_directory("map", filePathes, "\\*.fcf");

    int fileIndex = 0;
    bool key[INVALID + 1] = { false };
    cout << filePathes.size() << " files loaded.\n";
    if (filePathes.size() == 0)
    {
        cout << "No files were found!\n";
        return 0;
    }
    while (true)
    {
        system("CLS");
        if (key[ENTER])
        {
            break;
        }
        else if (key[ESC])
        {
            exit(0);
        }
        cout << "Please select a map with Left and Right:\n";
        if (key[A] || key[LA])
        {
            fileIndex--;
        }
        else if (key[D] || key[RA])
        {
            fileIndex++;
        }
        fileIndex = (fileIndex + filePathes.size()) % filePathes.size();
        preview(filePathes[fileIndex]);
        cout << filePathes[fileIndex] << " \n";
        cout << "Press Enter to select the map!";
        getKey(key);
    }
    loadMapInformation(filePathes[fileIndex]);
    draw();
    return 1;
}

void customGame()
{
    dungeon.customMap();
    dungeon.printMap();
    hero.setHeroLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
    creatureTotal = 1;
    creatureNum = creatureTotal;
    creatures.push_back(Creature());
    creatures[0].setCreatureLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
    draw();
    generateTerrainAndRegenerateTerrain();
}

void preview(string mapPath)
{
    ifstream inStream;
    string stringT;
    stringstream ss;
    int mapWidth, mapHeight;
    inStream.open(mapPath);
    getline(inStream, stringT);
    ss << stringT;
    ss >> mapWidth >> mapHeight;
    for (int y = 1; y - 1 < mapHeight; y++)
    {
        getline(inStream, stringT);
        cout << stringT << "\n";
    }
    inStream.close();
    ss.str("");
    ss.clear();
}

void loadMapInformation(string mapPath)
{
    ifstream inStream;
    string fileString, stringT;
    vector<string> lineString;
    vector<int> creaturesPropertyIndex;
    stringstream ss;
    int lineIndex = 0, height, heroX = 1, heroY = 1;
    char wall, floor, heroSkin, creatureSkinT;
    creatureTotal = 0;
    inStream.open(mapPath);
    getline(inStream, stringT);
    lineString.push_back(stringT);
    ss << stringT;
    ss >> height >> height >> wall >> heroSkin;
    while (ss >> creatureSkinT)
    {
        creaturesSkinList.push_back(creatureSkinT);
    }
    floor = stringT[(int)stringT.length() - 1];
    while (getline(inStream, stringT))
    {
        lineString.push_back(stringT);
    }
    for (int y = 0; y < height; y++)
    {
        if (loadFindHeroLocation(heroX, lineString[y + 1], floor, heroSkin))
        {
            heroY = y;
        }
        while (loadFindCreatureLocation(creatures, y, lineString[y + 1], floor, creaturesSkinList, creaturesPropertyIndex));
    }
    inStream.close();
    lineIndex = dungeon.loadMap(lineString);
    lineIndex++;
    lineIndex = hero.loadHeroInformation(heroX, heroY, lineString, lineIndex, heroSkin);
    lineIndex++;
    for (int i = 0; i < creatureTotal; i++)
    {
        lineIndex = creatures[i].loadCreatureInformation(lineString, lineIndex, creaturesSkinList, creaturesPropertyIndex[i]);
    }
    cout << creatureTotal;
}

bool loadFindHeroLocation(int& heroX, string& stringT, char floor, char heroSkin)
{
    int x = stringT.find(heroSkin);
    if (x == -1)
    {
        return false;
    }
    heroX = x;
    stringT[heroX] = floor;
    true;
}

bool loadFindCreatureLocation(vector<Creature>& creature, int creatureY, string& stringT, char floor, vector<char>& creaturesSkinList, vector<int>& creaturesPropertyIndex)
{
    int creatureX;
    for (int i = 0; i < creaturesSkinList.size(); i++)
    {
        creatureX = stringT.find(creaturesSkinList[i]);
        if (creatureX != -1)
        {
            creature.push_back(Creature());
            creature[creatureTotal++].loadCreatureLocation(creatureX, creatureY);
            stringT[creatureX] = floor;
            creaturesPropertyIndex.push_back(i);
            return true;
        }
    }
    return false;
}

void generateTerrainAndRegenerateTerrain()
{
    bool confirmTerrain = false;
    cout << "Do you want to generate terrain? (y/n): ";
    char choose;
    while (!confirmTerrain)
    {
        while (true)
        {
            choose = _getch();
            if (choose == 'Y' || choose == 'y')
            {
                dungeon.fill();
                for (int i = 0; i < creatureTotal; i++)
                {
                    dungeon.generateTerrain(hero.getX(), hero.getY(), creatures[i].getX(), creatures[i].getY());
                }
                break;
            }
            else if (choose == 'N' || choose == 'n')
            {
                confirmTerrain = true;
                break;
            }
            else if (choose == 27) //ESC
            {
                exit(0);
            }
        }

        if (!confirmTerrain)
        {
            draw();
            cout << "Do you want to regenerate terrain? (y/n): ";
        }
    }
}

void creaturesTurn()
{
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creatures[i].isLive())
        {
            creatures[i].seeHero(hero.getX(), hero.getY());
            if (creatures[i].isAlert())
            {
                trackHero(hero.getX(), hero.getY(), creatures[i].getX(), creatures[i].getY(), creatures[i]);
            }
            else
            {
                creatures[i].energyRecovery();
            }
            creatures[i].seeHero(hero.getX(), hero.getY());
        }
    }
}

void trackHero(int heroX, int heroY, int creatureX, int creatureY, Creature& creatureT) //problem
{
    if (heroX == creatureX && heroY == creatureY)
    {
        return;
    }
    queue<Point> place;
    int dir;
    int nextX;
    int nextY;
    bool find = false;
    vector<vector<int> > flag;
    for (int j = 0; j < dungeon.getHeight(); j++)
    {
        flag.push_back(vector<int>(dungeon.getWidth(), 0));
    }
    for (dir = 0; dir < 4; dir++)
    {
        nextX = creatureX + dx[dir];
        nextY = creatureY + dy[dir];
        if (!dungeon.isBoundary(nextX, nextY) && !dungeon.isObstacle(nextX, nextY))
        {
            place.push(Point(nextX, nextY));
            flag[nextY][nextX] = 1;
            if (nextX == heroX && nextY == heroY)
            {
                find = true;
                break;
            }
        }
    }
    while (!find)
    {
        for (int i = 0; i < 4; i++)
        {
            dir = (dir + 1 + 4) % 4;
            nextX = place.front().X + dx[dir];
            nextY = place.front().Y + dy[dir];
            if (!dungeon.isBoundary(nextX, nextY) && !dungeon.isObstacle(nextX, nextY) && flag[nextY][nextX] == 0)
            {
                place.push(Point(nextX, nextY));
                flag[nextY][nextX] = flag[place.front().Y][place.front().X] + 1;
                if (nextX == heroX && nextY == heroY)
                {
                    find = true;
                    break;
                }
            }
        }
        place.pop();
    }
    int nowX = nextX;
    int nowY = nextY;
    int step = flag[nowY][nowX];
    while (step > 1)
    {
        for (dir = 0; dir < 4; dir++)
        {
            nextX = nowX + dx[dir];
            nextY = nowY + dy[dir];
            if (dungeon.isBoundary(nextX, nextY) || dungeon.isObstacle(nextX, nextY))
            {
                continue;
            }
            if (flag[nowY][nowX] - 1 == flag[nextY][nextX])
            {
                break;
            }
        }
        nowX = nextX;
        nowY = nextY;
        step--;
    }
    creatureT.move(nextX - creatureX, nextY - creatureY);
    place.empty();
    flag.empty();
}

void heroBeDamaged()
{
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creatures[i].isLive() && hero.touchCreature(creatures[i].getX(), creatures[i].getY()))
        {
            InvincibleStart = hero.hurt(creatures[i].damage());
            InvincibleEnd = clock();
            if (InvincibleEnd - InvincibleStart > defaultHeroInvincibleTime)
            {
                hero.vincible();
            }
        }
    }
}

void creatureInformation()
{
    creatureNum = 0;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (!creatures[i].isLive())
        {
            continue;
        }
        if (creatureNum + 1 >= dungeon.getHeight() && creatureNum + 1 <= 40)
        {
            screen.push_back(string(dungeon.getWidth(), ' '));
        }
        if (creatureNum + 1 == 40)
        {
            screen[creatureNum + 1] += "  etc.";
        }
        else if(creatureNum + 1 < 40)
        {
            screen[creatureNum + 1] += "  " + creatures[i].information();
        }
        creatureNum++;
    }
    stringstream ss;
    ss << "There are only " << creatureNum << " Creatures left.";
    screen.push_back(ss.str());
}

void generateTrigger()
{
    int triggerX;
    int triggerY;
    do
    {
        triggerX = rand() % dungeon.getWidth();
        triggerY = rand() % dungeon.getHeight();
        
    } while (dungeon.isBoundary(triggerX, triggerY) || dungeon.isObstacle(triggerX, triggerY));
    triggers.push_back(Trigger(triggerX, triggerY));
    triggerTotal++;
}

void draw()
{
    screen = dungeon.outputMap();
    for (int i = 0; i < triggerTotal; i++)
    {
        if (triggers[i].isExist())
        {
            screen[triggers[i].getY()][triggers[i].getX()] = triggers[i].printTrigger();
        }
    }
    if (hero.isLive())
    {
        screen[hero.getY()][hero.getX()] = hero.getSkin();
    }
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creatures[i].isLive())
        {
            if (creatures[i].isAlert())
            {
                screen[creatures[i].getY()][creatures[i].getX()] = '!';
            }
            else
            {
                screen[creatures[i].getY()][creatures[i].getX()] = creatures[i].getSkin();
            }
        }
    }
    screen[0] += "  " + hero.information();
    creatureInformation();
    printScreen();
}

void printScreen()
{
    system("CLS");
    gameInformation();
    for (string line : screen)
    {
        cout << line << "\n";
    }
}

void isGameOver()
{
    bool key[INVALID + 1] = { false };
    if (!hero.isLive())
    {
        gameState = GAMEOVER;
        draw();
        cout << "\n";
        cout << "---------Game Over---------\n";
        cout << "Press ESC to leave.";
        do
        {
            getKey(key);
        } while (!key[ESC]);
        return;
    }

    gameState = GAMEOVER;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creatures[i].isLive())
        {
            gameState = GAMING;
            break;
        }
    }

    if (gameState == GAMEOVER)
    {
        draw();
        cout << "\n";
        cout << "----------You win----------\n";
        cout << "Press ESC to leave.";
        do
        {
            getKey(key);
        } while (!key[ESC]);
    }
}

bool DemonSlayer::isMount()
{
    vector<string> filePathes;
    LoadFiles loadFiles;
    loadFiles.read_directory("extensions", filePathes, "\\DemonSlayer.ext");
    return filePathes.size() > 0;
}

void DemonSlayer::BreathOfThunder()
{
    system("CLS");
    fontsize(128, 128);
    LoadFiles loadFiles;
    mciSendString(loadFiles.ConvertString("close mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("open \"extensions\\BreathOfThunder.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("play mp3"), NULL, 0, NULL);
    cout << "\n ";
    Sleep(2100);
    cout << "雷";
    Sleep(200);
    cout << "之";
    Sleep(200);
    cout << "呼";
    Sleep(200);
    cout << "吸";
    Sleep(1100);
    cout << "\n        ";
    cout << "壹";
    Sleep(200);
    cout << "之";
    Sleep(200);
    cout << "型";
    Sleep(1300);
    cout << "\n             ";
    cout << "霹";
    Sleep(200);
    cout << "靂";
    Sleep(200);
    cout << "一";
    Sleep(200);
    cout << "閃";
    Sleep(1000);
    fontsize(16, 16);
    draw();
    int nextX = hero.getX() + dx[hero.getSwordDirection()];
    int nextY = hero.getY() + dy[hero.getSwordDirection()];
    int lastX = hero.getX();
    int lastY = hero.getY();
    int creatureIndex = 0;
    int creatureHurtTotal = 0;
    vector<int> creatureTIndex;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creatures[i].isLive() && creatures[i].getX() == hero.getX() && creatures[i].getY() == hero.getY())
        {
            creatureTIndex.push_back(i);
            creatureHurtTotal++;
            if (hero.getSwordDirection() == SNorth || hero.getSwordDirection() == SSouth)
            {
                screen[hero.getY()][hero.getX()] = '|';
            }
            else
            {
                screen[hero.getY()][hero.getX()] = '-';
            }
        }
    }
    while (!dungeon.isBoundary(nextX, nextY) && !dungeon.isObstacle(nextX, nextY))
    {
        Sleep(1);
        hero.move(dx[hero.getSwordDirection()], dy[hero.getSwordDirection()]);
        if (creatureIndex < creatureHurtTotal)
        {
            screen[creatures[creatureTIndex[creatureIndex]].getY()][creatures[creatureTIndex[creatureIndex]].getX()] = '/';
            creatureIndex = creatureHurtTotal;
        }
        else
        {
            if (hero.getSwordDirection() == SNorth || hero.getSwordDirection() == SSouth)
            {
                screen[lastY][lastX] = '|';
            }
            else
            {
                screen[lastY][lastX] = '-';
            }
        }
        lastX = hero.getX();
        lastY = hero.getY();
        screen[lastY][lastX] = hero.getSkin();
        for (int i = 0; i < creatureTotal; i++)
        {
            if (creatures[i].isLive() && creatures[i].getX() == hero.getX() && creatures[i].getY() == hero.getY())
            {
                creatureTIndex.push_back(i);
                creatureHurtTotal++;
            }
        }
        nextX = hero.getX() + dx[hero.getSwordDirection()];
        nextY = hero.getY() + dy[hero.getSwordDirection()];
        printScreen();
    }
    for (int index : creatureTIndex)
    {
        creatures[index].hurt(100);
    }
    Sleep(1000);
}

void DemonSlayer::BreathOfWater1()
{
    system("CLS");
    fontsize(128, 128);
    LoadFiles loadFiles;
    mciSendString(loadFiles.ConvertString("close mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("open \"extensions\\BreathOfWater1.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("play mp3"), NULL, 0, NULL);
    cout << "\n ";
    Sleep(280);
    cout << "全";
    Sleep(200);
    cout << "集";
    Sleep(200);
    cout << "中";
    Sleep(1100);
    cout << "水";
    Sleep(200);
    cout << "之";
    Sleep(200);
    cout << "呼";
    Sleep(200);
    cout << "吸";
    Sleep(650);
    cout << "\n        ";
    cout << "壹";
    Sleep(150);
    cout << "之";
    Sleep(150);
    cout << "型";
    Sleep(600);
    cout << "\n             ";
    cout << "水";
    Sleep(150);
    cout << "面";
    Sleep(150);
    cout << "斬";
    Sleep(1000);
    fontsize(16, 16);
    draw();
    int nextX = hero.getX() + dx[hero.getSwordDirection()];
    int nextY = hero.getY() + dy[hero.getSwordDirection()];
    vector<int> creatureTIndex;
    bool findCreature = false;;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creatures[i].isLive() && creatures[i].getX() == hero.getX() && creatures[i].getY() == hero.getY())
        {
            creatureTIndex.push_back(i);
            findCreature = true;
        }
    }
    while (!dungeon.isBoundary(nextX, nextY) && !dungeon.isObstacle(nextX, nextY) && !findCreature)
    {
        //Sleep(1);
        screen[hero.getY()][hero.getX()] = dungeon.getFloorSkin();
        hero.move(dx[hero.getSwordDirection()], dy[hero.getSwordDirection()]);
        screen[hero.getY()][hero.getX()] = hero.getSkin();
        for (int i = 0; i < creatureTotal; i++)
        {
            if (creatures[i].isLive() && creatures[i].getX() == hero.getX() && creatures[i].getY() == hero.getY())
            {
                creatureTIndex.push_back(i);
                findCreature = true;
            }
        }
        nextX = hero.getX() + dx[hero.getSwordDirection()];
        nextY = hero.getY() + dy[hero.getSwordDirection()];
        printScreen();
    }
    for (int i = 0; i < creatureTotal; i++)
    {
        if (hero.getSwordDirection() == SNorth || hero.getSwordDirection() == SSouth)
        {
            if (creatures[i].isLive() && (creatures[i].getX() == hero.getX() - 1 || creatures[i].getX() == hero.getX() + 1) && creatures[i].getY() == hero.getY())
            {
                creatureTIndex.push_back(i);
                screen[creatures[i].getY()][creatures[i].getX()] = '/';
            }
        }
        else
        {
            if (creatures[i].isLive() && creatures[i].getX() == hero.getX() && (creatures[i].getY() == hero.getY() + 1 || creatures[i].getY() == hero.getY() - 1))
            {
                creatureTIndex.push_back(i);
                screen[creatures[i].getY()][creatures[i].getX()] = '/';
            }
        }
    }
    printScreen();
    Sleep(1000);
    for (int index : creatureTIndex)
    {
        creatures[index].hurt(100);
    }
}

void DemonSlayer::BreathOfWater11()
{
    system("CLS");
    fontsize(128, 128);
    LoadFiles loadFiles;
    mciSendString(loadFiles.ConvertString("close mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("open \"extensions\\BreathOfWater11.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("play mp3"), NULL, 0, NULL);
    cout << "\n ";
    Sleep(1100);
    cout << "全";
    Sleep(200);
    cout << "集";
    Sleep(200);
    cout << "中";
    Sleep(1100);
    cout << "\n    ";
    cout << "水";
    Sleep(200);
    cout << "之";
    Sleep(200);
    cout << "呼";
    Sleep(200);
    cout << "吸";
    Sleep(1400);
    cout << "\n         ";
    cout << "拾";
    Sleep(200);
    cout << "壹";
    Sleep(200);
    cout << "之";
    Sleep(200);
    cout << "型";
    Sleep(3000);
    cout << "\n              ";
    cout << "風";
    Sleep(100);
    cout << "平";
    Sleep(100);
    cout << "浪";
    Sleep(100);
    cout << "靜";
    Sleep(1000);
    fontsize(16, 16);
    draw();
    Sleep(500);
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creatures[i].isLive() && creatures[i].isAlert())
        {
            creatures[i].hurt(100);
            screen[creatures[i].getY()][creatures[i].getX()] = '/';
        }
    }
    printScreen();
    Sleep(1000);
}