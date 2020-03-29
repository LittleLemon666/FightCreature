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
#include "Trigger.h"
#include "Point.h"
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
void isGameOver();

int main()
{
    srand(time(NULL));
    ::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000); //full screen
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
    cout << "\n";
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
    LoadFCF::read_directory("map", filePathes);

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
    floor = stringT[(int)stringT.length() - 2];
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
        if (creatureNum + 1 >= dungeon.getHeight())
        {
            screen.push_back(string(dungeon.getWidth(), ' '));
        }
        screen[creatureNum + 1] += "  " + creatures[i].information();
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

    system("CLS");
    gameInformation();
    screen[0] += "  " + hero.information();
    creatureInformation();
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