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
#include "Dungeon.h"
#include "Hero.h"
#include "Creature.h"
#include "Point.h"
#define defaultCreatureNum 5
#define defaultCreatureSpeed 500
#define defaultHeroInvincibleTime 1000
using namespace std;
using std::ifstream;

int dx[] = { 0, 0, -1, 1 };
int dy[] = { -1, 1, 0, 0 };

Dungeon dungeon;
Hero hero;
vector<Creature> creature;
static int creatureTotal;
static int creatureNum;
static int gameState;
static clock_t InvincibleEnd;
static clock_t InvincibleStart;
vector<string> screen;

enum KeyState
{
    ESC,
    W,
    S,
    A,
    D,
    SPACE,
    LA,
    RA,
    ENTER,
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
    case 75:
        key[LA] = true;
        break;
    case 77:
        key[RA] = true;
        break;
    }
}

class LoadFCF
{
public:
    static LPWSTR ConvertString(const std::string& instr)
    {
        //Author: scruffybear
        //URL: https://xionghuilin.com/c-convert-between-string-and-cstring-lpwstr/

        // Assumes std::string is encoded in the current Windows ANSI codepage
        int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);

        if (bufferlen == 0)
        {
            // Something went wrong. Perhaps, check GetLastError() and log.
            return 0;
        }

        // Allocate new LPWSTR - must deallocate it later
        LPWSTR widestr = new WCHAR[bufferlen + 1];

        ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);

        // Ensure wide string is null terminated
        widestr[bufferlen] = 0;

        // Do something with widestr
        return widestr;
        //delete[] widestr;
    }

    static bool cvtLPW2stdstring(std::string& s, const LPWSTR pw, UINT codepage = CP_ACP)
    {
        //Author: ArkM
        //URL: https://www.daniweb.com/programming/software-development/threads/155420/lpwstr-to-std-string-help

        bool res = false;
        char* p = 0;
        int bsz;
        bsz = WideCharToMultiByte(codepage,
            0,
            pw, -1,
            0, 0,
            0, 0);
        if (bsz > 0) {
            p = new char[bsz];
            int rc = WideCharToMultiByte(codepage,
                0,
                pw, -1,
                p, bsz,
                0, 0);
            if (rc != 0) {
                p[bsz - 1] = 0;
                s = p;
                res = true;
            }
        }
        delete[] p;
        return res;
    }

    static void read_directory(const std::string& path, vector<string>& filePathes) //write map in this function
    {
        //Author: MARTIN
        //URL: http://www.martinbroadhurst.com/list-the-files-in-a-directory-in-c.html

        std::string pattern(path);
        pattern.append("\\*.fcf");
        string s;
        WIN32_FIND_DATA data;
        HANDLE hFind = FindFirstFile(ConvertString(pattern.c_str()), &data);
        do {
            cvtLPW2stdstring(s, data.cFileName);
            if (s.length() == 0 || s[0] == '?')
            {
                break;
            }
            cout << s << "\n";
            filePathes.push_back(path + "\\" + s);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
};

void creatureInformation()
{
    creatureNum = 0;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (!creature[i].isLive())
        {
            continue;
        }
        creatureNum++;
        creature[i].information();
    }
    cout << "There are only " << creatureNum << " Creatures left." << "\n";
}

void gameInformation()
{
    cout << "Press WASD or arrow keys to control Hero(icon H).\n";
    cout << "Press Space to attack in the direction where Hero is facing.\n";
    cout << "Avoid touching creatures (icon C) or Hero will take damage.\n";
    cout << "Use Hero's sword to knock out creatures!\n";
    cout << "Press ESC to exit the game.\n";
    cout << "\n";
}

void draw()
{
    screen = dungeon.outMap();
    if (hero.isLive())
    {
        screen[hero.getY()][hero.getX()] = 'H';
    }
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creature[i].isLive())
        {
            if (creature[i].getHeroDirection() == NotFound)
            {
                screen[creature[i].getY()][creature[i].getX()] = 'C';
            }
            else
            {
                screen[creature[i].getY()][creature[i].getX()] = '!';
            }
        }
    }

    system("CLS");
    gameInformation();
    for (string line : screen)
    {
        cout << line;
    }
    cout << "\n";
    hero.information();
    creatureInformation();
}

void isGameOver()
{
    if (!hero.isLive())
    {
        gameState = GAMEOVER;
        draw();
        cout << "\n";
        cout << "---------Game Over---------\n";
        return;
    }

    gameState = GAMEOVER;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creature[i].isLive())
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
    }
}

void trackHero(int heroX, int heroY, int creatureX, int creatureY, Creature& creatureT)
{
    if (heroX == creatureX && heroY == creatureY)
    {
        return;
    }
    queue<Point> place;
    int dir;
    for (dir = 0; dir < 4; dir++)
    {
        if (!dungeon.isBoundary(heroX + dx[dir], heroY + dy[dir]) &&!dungeon.isObstacle(heroX + dx[dir], heroY + dy[dir]))
        {
            place.push(Point(heroX + dx[dir], heroY + dy[dir]));
        }
    }

    bool find = false;
    vector<vector<bool> > flag;
    for (int j = 0; j < dungeon.getHeight(); j++)
    {
        flag.push_back(vector<bool>(dungeon.getWidth(), false));
    }
    while (!find)
    {
        for (int i = 0; i < 4; i++)
        {
            dir = (dir + 1 + 4) % 4;
            if (!dungeon.isBoundary(place.front().X + dx[dir], place.front().Y + dy[dir]) && !dungeon.isObstacle(place.front().X + dx[dir], place.front().Y + dy[dir]) && !flag[place.front().Y + dy[dir]][place.front().X + dx[dir]])
            {
                flag[place.front().Y + dy[dir]][place.front().X + dx[dir]] = true;
                if (place.front().X + dx[dir] == creatureX && place.front().Y + dy[dir] == creatureY)
                {
                    find = true;
                    break;
                }
                place.push(Point(place.front().X + dx[dir], place.front().Y + dy[dir]));
            }
        }
        place.pop();
    }
    creatureT.move(-dx[dir], -dy[dir]);
    place.empty();
    flag.empty();
}

void creaturesTurn()
{
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creature[i].isLive())
        {
            creature[i].seeHero(hero.getX(), hero.getY());
            if (creature[i].getState() == CAlert)
            {
                trackHero(hero.getX(), hero.getY(), creature[i].getX(), creature[i].getY(), creature[i]);
            }
            creature[i].seeHero(hero.getX(), hero.getY());
        }
    }
}

void heroBeDamaged()
{
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creature[i].isLive() && hero.touchCreature(creature[i].getX(), creature[i].getY()))
        {
            InvincibleStart = hero.hurt(creature[i].damage());
            InvincibleEnd = clock();
            if (InvincibleEnd - InvincibleStart > defaultHeroInvincibleTime)
            {
                hero.vincible();
            }
        }
    }
}

void update(bool key[])
{
    if (gameState == GAMING)
    {
        int _x = 0, _y = 0;
        if (key[W])
        {
            _x = 0;
            _y = -1;
        }
        else if (key[S])
        {
            _x = 0;
            _y = 1;
        }
        else if (key[A])
        {
            _x = -1;
            _y = 0;
        }
        else if (key[D])
        {
            _x = 1;
            _y = 0;
        }
        else if (key[SPACE])
        {
            for (int i = 0; i < creatureTotal; i++)
            {
                if (creature[i].isLive())
                {
                    hero.getExp(creature[i].hurt(hero.slash(creature[i].getX(), creature[i].getY())));
                }
            }
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
        }
    }
}

bool findHeroLocation(int& heroX, string& stringT, char floor)
{
    int x = stringT.find('H');
    if (x == -1)
    {
        return false;
    }
    heroX = x;
    stringT[heroX] = floor;
    true;
}

bool findCreatureLocation(vector<Creature>& creature, int creatureY, string& stringT, char floor)
{
    int creatureX = stringT.find('C');
    if (creatureX != -1)
    {
        creature.push_back(Creature());
        creature[creatureTotal++].setCreatureLocation(creatureX, creatureY);
        stringT[creatureX] = floor;
        return true;
    }
    return false;
}

void loadMapInformation(string mapPath)
{
    ifstream inStream;
    string fileString, stringT;
    vector<string> lineString;
    stringstream ss;
    int lineIndex = 0, height, heroX = 1, heroY = 1;
    char wall, floor;
    creatureTotal = 0;
    inStream.open(mapPath);
    getline(inStream, stringT);
    lineString.push_back(stringT);
    ss << stringT;
    ss >> height >> height >> wall;
    floor = stringT[(int)stringT.length() - 2];
    while (getline(inStream, stringT))
    {
        lineString.push_back(stringT);
    }
    for (int y = 0; y < height; y++)
    {
        if (findHeroLocation(heroX, lineString[y + 1], floor))
        {
            heroY = y;
        }
        while (findCreatureLocation(creature, y, lineString[y + 1], floor));
    }
    inStream.close();
    lineIndex = dungeon.loadMap(lineString);
    lineIndex++;
    lineIndex = hero.loadHeroInformation(heroX, heroY, lineString, lineIndex);
    lineIndex++;
    for (int i = 0; i < creatureTotal; i++)
    {
        lineIndex = creature[i].setCreatureInformation(lineString, lineIndex);
    }
    cout << creatureTotal;
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
    for (int y = 1; y - 1 < mapHeight;y++)
    {
        getline(inStream, stringT);
        cout << stringT << "\n";
    }
    inStream.close();
    ss.str("");
    ss.clear();
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
                    dungeon.generateTerrain(hero.getX(), hero.getY(), creature[i].getX(), creature[i].getY());
                }
                break;
            }
            else if (choose == 'N' || choose == 'n')
            {
                confirmTerrain = true;
                break;
            }
        }

        if (!confirmTerrain)
        {
            draw();
            cout << "Do you want to regenerate terrain? (y/n): ";
        }
    }
}

void quickGame()
{
    vector<Point> point;
    dungeon.generateMap();
    dungeon.printMap();
    hero.setHeroLocation(dungeon.getWidth(), dungeon.getHeight(), QuickGame);
    creatureTotal = defaultCreatureNum;
    creatureNum = creatureTotal;
    for (int i = 0; i < creatureTotal; i++)
    {
        creature.push_back(Creature());
        creature[i].setCreatureLocation(dungeon.getWidth(), dungeon.getHeight(), QuickGame);
        point.push_back(Point(creature[i].getX(), creature[i].getY()));
    }
    dungeon.generateTerrain(hero.getX(), hero.getY(), point, creatureTotal);
    draw();
    generateTerrainAndRegenerateTerrain();
}

int loadGame()
{
    vector<string> filePathes;
    LoadFCF::read_directory("map", filePathes);

    int fileIndex = 0;
    bool key[INVALID + 1] = { false };
    cout << filePathes.size() << " files loaded.\n";
    if (filePathes.size() == 0)
    {
        cout << "No files were found!\n";
        return 0;
    }
    cout << "Please select a map with Left and Right:\n";
    preview(filePathes[fileIndex]);
    cout << "-"<<filePathes[fileIndex] << " \n";
    cout << "Press Enter to select the map!";
    while (true)
    {
        getKey(key);
        system("CLS");
        if (key[ENTER])
        {
            break;
        }
        cout << "Please select a map with Left and Right:\n";
        if (key[LA])
        {
            fileIndex--;
        }
        else if (key[RA])
        {
            fileIndex++;
        }
        fileIndex = (fileIndex + filePathes.size()) % filePathes.size();
        preview(filePathes[fileIndex]);
        cout << filePathes[fileIndex] << " \n";
        cout << "Press Enter to select the map!";
    }
    loadMapInformation(filePathes[fileIndex]);
    draw();
    return 1;
}

void customGame()
{
    dungeon.inputMap();
    dungeon.printMap();
    hero.setHeroLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
    creatureTotal = 1;
    creatureNum = creatureTotal;
    creature.push_back(Creature());
    creature[0].setCreatureLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
    draw();
    generateTerrainAndRegenerateTerrain();
}

void menu()
{
    cout << "Quick game please press 1.\n";
    cout << "Load game please press 2.\n";
    cout << "Custom game please press 3.\n";
    int result;
    while (true)
    {
        char choose = _getch();
        
        switch (choose)
        {
        case '1':
            quickGame();
            return;

        case '2':
            result = loadGame();
            if (result == 0)
            {
                menu();
            }
            return;

        case '3':
            customGame();
            return;
        }
    }
}

int main()
{
    srand(time(NULL));
    menu();
    gameState = PREPARING;

    bool key[INVALID + 1] = { false };

    update(key);
    gameState = GAMING;
    clock_t begin = clock();
    clock_t end;
    do
    {
        if (_kbhit())
        {
            getKey(key);
            update(key);
        }
        end = clock();
        if (end - begin > defaultCreatureSpeed)
        {
            creaturesTurn();
            begin = clock();
        }
        heroBeDamaged();
        draw();
        isGameOver();
    }while (!key[ESC] && gameState != GAMEOVER);

    cout << "Press ESC to leave.";
    key[ESC] = false;
    while (!key[ESC])
    {
        getKey(key);
    }
    return 0;
}