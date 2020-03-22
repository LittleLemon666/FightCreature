#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <vector>
#include <queue>
#include <conio.h>		// _getch() <= to read input without enter "ENTER" key
#include "Dungeon.h"
#include "Hero.h"
#include "Creature.h"
#include "Point.h"
using namespace std;

int dx[] = { 0, 0, -1, 1 };
int dy[] = { -1, 1, 0, 0 };

Dungeon dungeon;
Hero hero;
Creature creature;
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
    if (hero.getState() != HDeath)
    {
        screen[hero.getY()][hero.getX()] = 'H';
    }
    else
    {
        screen[hero.getY()][hero.getX()] = dungeon.getFloor();
    }

    if (creature.getHeroDirection() == Unknown)
    {
        screen[creature.getY()][creature.getX()] = 'C';
    }
    else if (creature.getState() == CDeath)
    {
        screen[creature.getY()][creature.getX()] = dungeon.getFloor();
    }
    else
    {
        screen[creature.getY()][creature.getX()] = '!';
    }

    if (hero.getState() != HDeath)
    {
        screen[hero.getY()][hero.getX()] = 'H';
    }
    else
    {
        screen[hero.getY()][hero.getX()] = dungeon.getFloor();
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
    cout << "Creature health:" << setw(creature.getHealth() + (streamsize)1) << setfill('#') << " " << creature.getHealth();
    cout << " Energy:" << setw(creature.getEnergy() + (streamsize)1) << setfill('#') << " " << creature.getEnergy();
    cout << "\n";
    if (creature.getState() == CAlert)
    {
        if (creature.getHeroDirection() == Beside)
        {
            cout << "The creature is beside you.";
        }
        else
        {
            cout << "Hero is in the ";
            switch (creature.getHeroDirection())
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
}

void gameOver()
{
    if (hero.getState() == HDeath)
    {
        gameState = GAMEOVER;
        draw();
        cout << "\n";
        cout << "---------Game Over---------\n";
    }
    else if (creature.getState() == CDeath)
    {
        gameState = GAMEOVER;
        draw();
        cout << "\n";
        cout << "----------You win----------\n";
    }
}

void trackHero(int heroX, int heroY, int creatureX, int creatureY)
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
    while (!find)
    {
        for (int i = 0; i < 4; i++)
        {
            dir = (dir + 1 + 4) % 4;
            if (!dungeon.isBoundary(place.front().X + dx[dir], place.front().Y + dy[dir]) && !dungeon.isObstacle(place.front().X + dx[dir], place.front().Y + dy[dir]))
            {
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
    creature.move(-dx[dir], -dy[dir]);
    place.empty();
}

void update(bool key[])
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
        creature.hurt(hero.slash(creature.getX(), creature.getY()));
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

    if (!dungeon.isBoundary(hero.getX() + _x, hero.getY() + _y) &&!dungeon.isObstacle(hero.getX() + _x, hero.getY() + _y))
    {
        hero.move(_x, _y);
    }

    creature.seeHero(hero.getX(), hero.getY());
    if (creature.getState() == CAlert)
    {
        trackHero(hero.getX(), hero.getY(), creature.getX(), creature.getY());
    }
    creature.seeHero(hero.getX(), hero.getY());

    if (hero.touchCreature(creature.getX(), creature.getY()))
    {
        hero.hurt(creature.damage());
    }
    draw();
    gameOver();
}

void menu()
{
    cout << "Quick game please press 1.\n";
    cout << "Load game please press 2.\n";
    cout << "Custom game please press 3.\n";
    while (true)
    {
        char choose = _getch();
        switch (choose)
        {
        case '1':
            dungeon.generateMap();
            dungeon.printMap();
            hero.setHeroLocation(dungeon.getWidth(), dungeon.getHeight(), QuickGame);
            creature.setCreatureLocation(dungeon.getWidth(), dungeon.getHeight(), QuickGame);
            dungeon.generateTerrain(hero.getX(), hero.getY(), creature.getX(), creature.getY());
            draw();
            return;
        case '2':
            return;
        case '3':
            dungeon.inputMap();
            dungeon.printMap();
            hero.setHeroLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
            creature.setCreatureLocation(dungeon.getWidth(), dungeon.getHeight(), CustomGame);
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
                        dungeon.generateTerrain(hero.getX(), hero.getY(), creature.getX(), creature.getY());
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
    
    gameState = GAMING;

    bool key[6];

    update(key);
    do
    {
        getKey(key);
        update(key);
    }while (!key[ESC] && gameState != GAMEOVER);

    return 0;
}