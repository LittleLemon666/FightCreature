#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <conio.h>		// _getch() <= to read input without enter "ENTER" key
#include "Dungeon.h"
#include "Hero.h"
#include "Creature.h"
#include "Point.h"
#define defaultCreatureNum 5
using namespace std;

int dx[] = { 0, 0, -1, 1 };
int dy[] = { -1, 1, 0, 0 };

Dungeon dungeon;
Hero hero;
vector<Creature> creature;
int creatureTotal;
int creatureNum;
int gameState;
vector<string> screen;

enum KeyState
{
    ESC,
    W,
    S,
    A,
    D,
    SPACE,
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
    }
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
            if (creature[i].getHeroDirection() == Unknown)
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
    //information
    cout << "Press WASD to control Hero(icon H).\n";
    cout << "Press Space to attack Creature(icon C).\n";
    cout << "Press ESC to exit the game.\n";
    cout << "Avoid touch Creature or Hero will be damage.\n";
    cout << "The direction of the sword that Hero taking is on where Hero facing.\n";
    cout << "Use Hero's sword to knock out the Creature!\n";
    cout << "\n";

    for (string line : screen)
    {
        cout << line;
    }
    cout << "\n";
    cout << "Hero health:" << setw(hero.getHealth() + (streamsize)1) << setfill('#') << " " << hero.getHealth();
    cout << " Hero is facing ";
    switch (hero.getSwordDirection())
    {
    case SNorth:
        cout << "North";
        break;
    case SSouth:
        cout << "South";
        break;
    case SEast:
        cout << "East";
        break;
    case SWest:
        cout << "West";
        break;
    }

    cout << "\n";
    creatureNum = 0;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creature[i].getState() == CDeath)
        {
            continue;
        }
        creatureNum++;
        cout << "Creature health:" << setw(creature[i].getHealth() + (streamsize)1) << setfill('#') << " " << creature[i].getHealth();
        cout << " Energy:" << setw(creature[i].getEnergy() + (streamsize)1) << setfill('#') << " " << creature[i].getEnergy();
        cout << " ";
        if (creature[i].getState() == CAlert)
        {
            if (creature[i].getHeroDirection() == Beside)
            {
                cout << "The creature is beside you.";
            }
            else
            {
                cout << "Hero is in the ";
                switch (creature[i].getHeroDirection())
                {
                case East:
                    cout << "east";
                    break;
                case West:
                    cout << "west";
                    break;
                case North:
                    cout << "north";
                    break;
                case South:
                    cout << "south";
                    break;
                case NorthEast:
                    cout << "northeast";
                    break;
                case SouthEast:
                    cout << "southeast";
                    break;
                case NorthWest:
                    cout << "northwest";
                    break;
                case SouthWest:
                    cout << "southwest";
                    break;
                }
            }
        }
        cout << "\n";
    }
    cout << "There are only " << creatureNum << " Creatures left." << "\n";
}

void gameOver()
{
    if (!hero.isLive())
    {
        gameState = GAMEOVER;
        draw();
        cout << "\n";
        cout << "---------Game Over---------\n";
    }
    return;

    gameState = GAMEOVER;
    for (int i = 0; i < creatureTotal; i++)
    {
        if (creature[i].isLive())
        {
            gameState = GAMING;
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
    std::queue<Point> place;
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
                    creature[i].hurt(hero.slash(creature[i].getX(), creature[i].getY()));
                }
            }
        }
        else
        {
            return;
        }
        gameOver();
        if (gameState == GAMEOVER)
        {
            return;
        }

        if (!dungeon.isBoundary(hero.getX() + _x, hero.getY() + _y) && !dungeon.isObstacle(hero.getX() + _x, hero.getY() + _y))
        {
            hero.move(_x, _y);
        }

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

                if (hero.touchCreature(creature[i].getX(), creature[i].getY()))
                {
                    hero.hurt(creature[i].damage());
                }
            }
        }
    }
    draw();
    gameOver();
}

void menu()
{
    cout << "Quick game please press 1.\n";
    cout << "Load game please press 2.\n";
    cout << "Custom game please press 3.\n";
    vector<Point> point;
    while (true)
    {
        char choose = _getch();
        switch (choose)
        {
        case '1':
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
            return;
        case '2':
            return;
        case '3':
            dungeon.inputMap();
            dungeon.printMap();
            hero.setHeroLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
            creatureTotal = 1;
            creatureNum = creatureTotal;
            creature.push_back(Creature());
            creature[0].setCreatureLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
            draw();
            bool confirmTerrain = false;
            std::cout << "Do you want to generate terrain? (y/n): ";
            while (!confirmTerrain)
            {
                while (true)
                {
                    choose = _getch();
                    if (choose == 'Y' || choose == 'y')
                    {
                        dungeon.generateTerrain(hero.getX(), hero.getY(), creature[0].getX(), creature[0].getY());
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
            
            return;
        }
    }
}

int main()
{
    srand(time(NULL));
    menu();
    gameState = PREPARING;

    bool key[6];

    update(key);
    gameState = GAMING;
    do
    {
        getKey(key);
        update(key);
    }while (!key[ESC] && gameState != GAMEOVER);

    return 0;
}