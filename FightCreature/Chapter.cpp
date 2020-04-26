#include "Chapter.h"
using namespace std;

int Chapter::gameState;
bool Chapter::isMountDemonSlayer;
Chapter::Chapter()
{
    dungeons.push_back(Dungeon());
    isMountDemonSlayer = DemonSlayer_isMount(); //extension
}

Chapter::~Chapter()
{
}

const void Chapter::gameInformation()
{
    cout << "Press WASD or arrow keys to control Hero(icon " << hero.getSkin() << ").\n";
    cout << "Press Space(sword) or J(arrow) to attack in the direction where Hero is facing.\n";
    cout << "Avoid touching enemies (icon ";
    for (int creaturesSkinIndex = 0; creaturesSkinIndex < dungeons[Dungeon::dungeonIndex].creaturesSkinList.size(); creaturesSkinIndex++)
    {
        cout << dungeons[Dungeon::dungeonIndex].creaturesSkinList[creaturesSkinIndex];
    }
    cout << ") or Hero will take damage.\n";
    cout << "Pick up triggers(+) will get some exp.\n";
    cout << "Enemies track Hero when Hero touches traps(" << string(1, Trap::trapShape) << ").\n";
    cout << "Use Hero's sword to knock out creatures!\n";
    cout << "Press ESC to exit the game.\n";
}

const void Chapter::creatureInformation()
{
    Creature::creatureNum = 0;
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (!dungeons[Dungeon::dungeonIndex].creatures[i].isLive())
        {
            continue;
        }
        if (Creature::creatureNum + 1 >= dungeons[Dungeon::dungeonIndex].getHeight() && Creature::creatureNum + 1 <= 40)
        {
            screen.push_back(string(dungeons[Dungeon::dungeonIndex].getWidth(), ' '));
        }
        if (Creature::creatureNum + 1 == 40)
        {
            screen[Creature::creatureNum + 1] += "  etc.";
        }
        else if (Creature::creatureNum + 1 < 40)
        {
            screen[Creature::creatureNum + 1] += "  " + dungeons[Dungeon::dungeonIndex].creatures[i].information();
        }
        Creature::creatureNum++;
    }
    screen.push_back("There are only " + to_string(Creature::creatureNum) + " Creatures left.");
}

const void Chapter::update(bool key[])
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
            heroSwordAttack();
        }
        else if (key[J])
        {
            heroBowAttack();
        }
        else if (isMountDemonSlayer && key[ZERO])
        {
            DemonSlayer_BreathOfWater11();
        }
        else if (isMountDemonSlayer && key[ONE])
        {
            DemonSlayer_BreathOfThunder();
        }
        else if (isMountDemonSlayer && key[TWO])
        {
            DemonSlayer_BreathOfWater1();
        }
        else
        {
            return;
        }
        isGameOver(key);
        if (gameState == GAMEOVER)
        {
            return;
        }

        heroMove(Point(_x, _y));
        objectAction();
    }
}

const void Chapter::quickGame()
{
    vector<Point> point;
    dungeons[Dungeon::dungeonIndex].generatePlain();
    hero.setHeroLocation(Point(dungeons[Dungeon::dungeonIndex].getWidth(), dungeons[Dungeon::dungeonIndex].getHeight()), QuickGame);
    Creature::creatureTotal = defaultcreatureNum;
    Creature::creatureNum = Creature::creatureTotal;
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        dungeons[Dungeon::dungeonIndex].creatures.push_back(Creature());
        dungeons[Dungeon::dungeonIndex].creatures[i].setCreatureLocation(Point(dungeons[Dungeon::dungeonIndex].getWidth(), dungeons[Dungeon::dungeonIndex].getHeight()), QuickGame);
        point.push_back(Point(dungeons[Dungeon::dungeonIndex].creatures[i].getX(), dungeons[Dungeon::dungeonIndex].creatures[i].getY()));
    }
    dungeons[Dungeon::dungeonIndex].generateTerrain(hero.getPosition(), point, Creature::creatureTotal);
    draw();
    generateTerrainAndRegenerateTerrain();
}

const int Chapter::loadGame(bool key[])
{
    vector<string> filePathes;
    LoadFiles loadFiles;
    loadFiles.read_directory("map", filePathes, "\\*.fcf");

    int fileIndex = 0;
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

const void Chapter::customGame()
{
    dungeons[Dungeon::dungeonIndex].customMap();
    hero.setHeroLocation(Point(dungeons[Dungeon::dungeonIndex].getWidth(), dungeons[Dungeon::dungeonIndex].getHeight()), CustomGame);
    Creature::creatureTotal = 1;
    Creature::creatureNum = Creature::creatureTotal;
    dungeons[Dungeon::dungeonIndex].creatures.push_back(Creature());
    dungeons[Dungeon::dungeonIndex].creatures[0].setCreatureLocation(Point(dungeons[Dungeon::dungeonIndex].getWidth(), dungeons[Dungeon::dungeonIndex].getHeight()), CustomGame);
    draw();
    generateTerrainAndRegenerateTerrain();
}

const void Chapter::generateTerrainAndRegenerateTerrain()
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
                dungeons[Dungeon::dungeonIndex].fill();
                for (int i = 0; i < Creature::creatureTotal; i++)
                {
                    dungeons[Dungeon::dungeonIndex].generateTerrain(hero.getPosition(), dungeons[Dungeon::dungeonIndex].creatures[i].getPosition());
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

const void Chapter::gaming(bool key[])
{
    gameState = PREPARING;
    update(key);
    gameState = GAMING;
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
        arrowsTurn();
        if (Creature::canCreaturesTurn())
        {
            creaturesTurn();
        }
        if (Trigger::canGenerate())
        {
            generateTrigger();
        }
        heroBeDamaged();
        draw();
        isGameOver(key);
    } while (!key[ESC] && gameState != GAMEOVER);
}

const void Chapter::preview(const string mapPath)
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

const void Chapter::loadMapInformation(const string mapPath)
{
    ifstream inStream;
    string fileString, stringT;
    vector<string> lineString;
    vector<int> creaturesPropertyIndex;
    stringstream ss;
    int lineIndex = 0, height, heroX = 1, heroY = 1;
    char wall, floor, heroSkin, creatureSkinT, trapSkin;
    Creature::creatureTotal = 0;
    inStream.open(mapPath);
    getline(inStream, stringT);
    lineString.push_back(stringT);
    ss << stringT;
    ss >> height >> height >> wall >> trapSkin >> heroSkin;
    while (ss >> creatureSkinT)
    {
        dungeons[Dungeon::dungeonIndex].creaturesSkinList.push_back(creatureSkinT);
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
        while (loadFindCreatureLocation(dungeons[Dungeon::dungeonIndex].creatures, y, lineString[y + 1], floor, dungeons[Dungeon::dungeonIndex].creaturesSkinList, creaturesPropertyIndex));
        while (loadFindTrapLocation(dungeons[Dungeon::dungeonIndex].traps, y, lineString[y + 1], floor, trapSkin));
    }
    inStream.close();
    lineIndex = dungeons[Dungeon::dungeonIndex].loadMap(lineString);
    lineIndex++;
    lineIndex = hero.loadHeroInformation(heroX, heroY, lineString, lineIndex, heroSkin);
    lineIndex++;
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        lineIndex = dungeons[Dungeon::dungeonIndex].creatures[i].loadCreatureInformation(lineString, lineIndex, dungeons[Dungeon::dungeonIndex].creaturesSkinList, creaturesPropertyIndex[i]);
    }
    cout << Creature::creatureTotal;
}

const bool Chapter::loadFindHeroLocation(int& heroX, string& stringT, const char floor, const char heroSkin)
{
    int x = stringT.find(heroSkin);
    if (x == -1)
    {
        return false;
    }
    heroX = x;
    stringT[heroX] = floor;
    return true;
}

const bool Chapter::loadFindCreatureLocation(vector<Creature>& creature, const int creatureY, string& stringT, const char floor, vector<char>& creaturesSkinList, vector<int>& creaturesPropertyIndex)
{
    int creatureX;
    for (int creatureSkinIndex = 0; creatureSkinIndex < creaturesSkinList.size(); creatureSkinIndex++)
    {
        creatureX = stringT.find(creaturesSkinList[creatureSkinIndex]);
        if (creatureX != -1)
        {
            creature.push_back(Creature());
            creature[Creature::creatureTotal++].loadCreatureLocation(Point(creatureX, creatureY));
            stringT[creatureX] = floor;
            creaturesPropertyIndex.push_back(creatureSkinIndex);
            return true;
        }
    }
    return false;
}

const bool Chapter::loadFindTrapLocation(vector<Trap>& traps, const int trapY, string& stringT, const char floor, const char trapSkin)
{
    int trapX = stringT.find(trapSkin);
    if (trapX != -1)
    {
        traps.push_back(Trap(trapX, trapY, trapSkin));
        stringT[trapX] = floor;
        return true;
    }
    return false;
}

const void Chapter::heroSwordAttack()
{
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive())
        {
            hero.getExp(dungeons[Dungeon::dungeonIndex].creatures[i].hurt(hero.slash(dungeons[Dungeon::dungeonIndex].creatures[i].getPosition())));
        }
    }
    for (int i = 0; i < Trap::trapTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].traps[i].isExist())
        {
            dungeons[Dungeon::dungeonIndex].traps[i].destroyTrap(hero.slash(dungeons[Dungeon::dungeonIndex].traps[i].getPosition()) > 0);
        }
    }
}

const void Chapter::heroBowAttack()
{
    arrows.push_back(Arrow(hero.getSwordDirection(), hero.getPosition()));
    int arrowX = arrows.back().getX();
    int arrowY = arrows.back().getY();
    if (dungeons[Dungeon::dungeonIndex].isBoundary(Point(arrowX, arrowY)) || dungeons[Dungeon::dungeonIndex].isObstacle(Point(arrowX, arrowY)))
    {
        arrows.back().arrowEnd();
    }
}

const void Chapter::heroMove(const Point dxy)
{
    if (!dungeons[Dungeon::dungeonIndex].isBoundary(Point(hero.getX() + dxy.X, hero.getY() + dxy.Y)) && !dungeons[Dungeon::dungeonIndex].isObstacle(Point(hero.getX() + dxy.X, hero.getY() + dxy.Y)))
    {
        hero.move(dxy);
    }
}

const void Chapter::creaturesTurn()
{
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive())
        {
            dungeons[Dungeon::dungeonIndex].creatures[i].seeHero(hero.getPosition());
            if (dungeons[Dungeon::dungeonIndex].creatures[i].isAlert() || Trap::trapIsTouched)
            {
                trackHero(hero.getPosition(), dungeons[Dungeon::dungeonIndex].creatures[i].getPosition(), dungeons[Dungeon::dungeonIndex].creatures[i]);
            }
            else
            {
                dungeons[Dungeon::dungeonIndex].creatures[i].energyRecovery();
            }
            dungeons[Dungeon::dungeonIndex].creatures[i].seeHero(hero.getPosition());
        }
    }
}

const void Chapter::trackHero(const Point heroPosition, const Point creaturePosition, Creature& creatureT) //problem
{
    if (heroPosition.X == creaturePosition.X && heroPosition.Y == creaturePosition.Y)
    {
        return;
    }
    queue<Point> place;
    int dir;
    int nextX;
    int nextY;
    bool find = false;
    vector<vector<int> > flag;
    for (int j = 0; j < dungeons[Dungeon::dungeonIndex].getHeight(); j++)
    {
        flag.push_back(vector<int>(dungeons[Dungeon::dungeonIndex].getWidth(), 0));
    }
    for (dir = 0; dir < 4; dir++)
    {
        nextX = creaturePosition.X + dx[dir];
        nextY = creaturePosition.Y + dy[dir];
        if (!dungeons[Dungeon::dungeonIndex].isBoundary(Point(nextX, nextY)) && !dungeons[Dungeon::dungeonIndex].isObstacle(Point(nextX, nextY)))
        {
            place.push(Point(nextX, nextY));
            flag[nextY][nextX] = 1;
            if (nextX == heroPosition.X && nextY == heroPosition.Y)
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
            if (!dungeons[Dungeon::dungeonIndex].isBoundary(Point(nextX, nextY)) && !dungeons[Dungeon::dungeonIndex].isObstacle(Point(nextX, nextY)) && flag[nextY][nextX] == 0)
            {
                place.push(Point(nextX, nextY));
                flag[nextY][nextX] = flag[place.front().Y][place.front().X] + 1;
                if (nextX == heroPosition.X && nextY == heroPosition.Y)
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
            if (dungeons[Dungeon::dungeonIndex].isBoundary(Point(nextX, nextY)) || dungeons[Dungeon::dungeonIndex].isObstacle(Point(nextX, nextY)))
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
    creatureT.move(Point(nextX - creaturePosition.X, nextY - creaturePosition.Y));
}

const void Chapter::heroBeDamaged()
{
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && hero.touchCreature(dungeons[Dungeon::dungeonIndex].creatures[i].getPosition()))
        {
            hero.hurt(dungeons[Dungeon::dungeonIndex].creatures[i].damage());
        }
    }
}

const void Chapter::generateTrigger()
{
    int triggerX;
    int triggerY;
    do
    {
        triggerX = rand() % dungeons[Dungeon::dungeonIndex].getWidth();
        triggerY = rand() % dungeons[Dungeon::dungeonIndex].getHeight();

    } while (dungeons[Dungeon::dungeonIndex].isBoundary(Point(triggerX, triggerY)) || dungeons[Dungeon::dungeonIndex].isObstacle(Point(triggerX, triggerY)));
    dungeons[Dungeon::dungeonIndex].triggers.push_back(Trigger(triggerX, triggerY));
    Trigger::triggerTotal++;
}

const void Chapter::arrowsTurn()
{
    int arrowNextStepX, arrowNextStepY;
    for (int arrowIndex = 0; arrowIndex < Arrow::arrowTotal; arrowIndex++)
    {
        arrows[arrowIndex].nextStep(arrowNextStepX, arrowNextStepY);
        if (arrows[arrowIndex].isExist())
        {
            if (dungeons[Dungeon::dungeonIndex].isBoundary(Point(arrowNextStepX, arrowNextStepY)) || dungeons[Dungeon::dungeonIndex].isObstacle(Point(arrowNextStepX, arrowNextStepY)))
            {
                arrows[arrowIndex].arrowEnd();
                continue;
            }
            arrows[arrowIndex].arrowMove();
            for (int creatureIndex = 0; creatureIndex < Creature::creatureTotal && arrows[arrowIndex].isExist(); creatureIndex++)
            {
                if (dungeons[Dungeon::dungeonIndex].creatures[creatureIndex].isLive())
                {
                    hero.getExp(dungeons[Dungeon::dungeonIndex].creatures[creatureIndex].hurt(arrows[arrowIndex].damage(dungeons[Dungeon::dungeonIndex].creatures[creatureIndex].getPosition())));
                }
            }
        }
    }
}

const void Chapter::objectAction()
{
    for (int i = 0; i < Trigger::triggerTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].triggers[i].isExist() && dungeons[Dungeon::dungeonIndex].triggers[i].getX() == hero.getX() && dungeons[Dungeon::dungeonIndex].triggers[i].getY() == hero.getY())
        {
            hero.getExp(dungeons[Dungeon::dungeonIndex].triggers[i].getTrigger());
        }
    }
    for (int i = 0; i < Trap::trapTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].traps[i].isExist() && dungeons[Dungeon::dungeonIndex].traps[i].getX() == hero.getX() && dungeons[Dungeon::dungeonIndex].traps[i].getY() == hero.getY())
        {
            dungeons[Dungeon::dungeonIndex].traps[i].touchTrap();
        }
    }
}
//creatureTotal should in array
const void Chapter::draw()
{
    screen = dungeons[Dungeon::dungeonIndex].outputMap();
    for (int i = 0; i < Trigger::triggerTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].triggers[i].isExist())
        {
            screen[dungeons[Dungeon::dungeonIndex].triggers[i].getY()][dungeons[Dungeon::dungeonIndex].triggers[i].getX()] = dungeons[Dungeon::dungeonIndex].triggers[i].printTrigger();
        }
    }
    for (int i = 0; i < Trap::trapTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].traps[i].isExist())
        {
            screen[dungeons[Dungeon::dungeonIndex].traps[i].getY()][dungeons[Dungeon::dungeonIndex].traps[i].getX()] = dungeons[Dungeon::dungeonIndex].traps[i].printTrap();
        }
    }
    for (int arrowIndex = 0; arrowIndex < Arrow::arrowTotal; arrowIndex++)
    {
        if (arrows[arrowIndex].isExist())
        {
            screen[arrows[arrowIndex].getY()][arrows[arrowIndex].getX()] = arrows[arrowIndex].getShape();
        }
    }
    if (hero.isLive())
    {
        screen[hero.getY()][hero.getX()] = hero.getSkin();
    }
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive())
        {
            if (dungeons[Dungeon::dungeonIndex].creatures[i].isAlert())
            {
                screen[dungeons[Dungeon::dungeonIndex].creatures[i].getY()][dungeons[Dungeon::dungeonIndex].creatures[i].getX()] = '!';
            }
            else
            {
                screen[dungeons[Dungeon::dungeonIndex].creatures[i].getY()][dungeons[Dungeon::dungeonIndex].creatures[i].getX()] = dungeons[Dungeon::dungeonIndex].creatures[i].getSkin();
            }
        }
    }
    screen[0] += "  " + hero.information();
    creatureInformation();
    printScreen();
}

const void Chapter::printScreen()
{
    system("CLS");
    TextColor(WHITE, BLACK, out);
    gameInformation();
    for (int lineIndex = 0; lineIndex < screen.size(); lineIndex++)
    {
        if (lineIndex == hero.getY())
        {
            TextColor(WHITE, BLACK, out);
            cout << screen[lineIndex].substr(0, hero.getX());
            TextColor(LIGHTCYAN, BLACK, out);
            cout << screen[lineIndex][hero.getX()];
            TextColor(WHITE, BLACK, out);
            cout << screen[lineIndex].substr(hero.getX() + 1, screen[lineIndex].length() - hero.getX()) << "\n";
        }
        else
        {
            TextColor(WHITE, BLACK, out);
            cout << screen[lineIndex] << "\n";
        }
    }
}

const void Chapter::isGameOver(bool key[])
{
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
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive())
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
//DemonSlayer
const bool Chapter::DemonSlayer_isMount() const
{
    vector<string> filePathes;
    LoadFiles loadFiles;
    loadFiles.read_directory("extensions", filePathes, "\\DemonSlayer.ext");
    return filePathes.size() > 0;
}

const void Chapter::DemonSlayer_BreathOfThunder()
{
    system("CLS");
    fontsize(128, 128, out);
    LoadFiles loadFiles;
    mciSendString(loadFiles.ConvertString("close mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("open \"extensions\\BreathOfThunder.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("play mp3"), NULL, 0, NULL);
    TextColor(YELLOW, BLACK, out);
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
    fontsize(16, 16, out);
    TextColor(WHITE, BLACK, out);
    draw();
    int nextX = hero.getX() + dx[hero.getSwordDirection()];
    int nextY = hero.getY() + dy[hero.getSwordDirection()];
    int lastX = hero.getX();
    int lastY = hero.getY();
    int creatureIndex = 0;
    int creatureHurtTotal = 0;
    vector<int> creatureTIndex;
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && dungeons[Dungeon::dungeonIndex].creatures[i].getX() == hero.getX() && dungeons[Dungeon::dungeonIndex].creatures[i].getY() == hero.getY())
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
    while (!dungeons[Dungeon::dungeonIndex].isBoundary(Point(nextX, nextY)) && !dungeons[Dungeon::dungeonIndex].isObstacle(Point(nextX, nextY)))
    {
        Sleep(1);
        hero.move(Point(dx[hero.getSwordDirection()], dy[hero.getSwordDirection()]));
        if (creatureIndex < creatureHurtTotal)
        {
            screen[dungeons[Dungeon::dungeonIndex].creatures[creatureTIndex[creatureIndex]].getY()][dungeons[Dungeon::dungeonIndex].creatures[creatureTIndex[creatureIndex]].getX()] = '/';
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
        for (int i = 0; i < Creature::creatureTotal; i++)
        {
            if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && dungeons[Dungeon::dungeonIndex].creatures[i].getX() == hero.getX() && dungeons[Dungeon::dungeonIndex].creatures[i].getY() == hero.getY())
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
        hero.getExp(dungeons[Dungeon::dungeonIndex].creatures[index].hurt(100));
    }
    Sleep(1000);
    clearInputBuffer();
}

const void Chapter::DemonSlayer_BreathOfWater1()
{
    system("CLS");
    fontsize(128, 128, out);
    LoadFiles loadFiles;
    mciSendString(loadFiles.ConvertString("close mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("open \"extensions\\BreathOfWater1.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("play mp3"), NULL, 0, NULL);
    TextColor(LIGHTCYAN, BLACK, out);
    cout << "\n ";
    Sleep(280);
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
    Sleep(650);
    cout << "\n         ";
    cout << "壹";
    Sleep(150);
    cout << "之";
    Sleep(150);
    cout << "型";
    Sleep(600);
    cout << "\n            ";
    cout << "水";
    Sleep(150);
    cout << "面";
    Sleep(150);
    cout << "斬";
    Sleep(1000);
    fontsize(16, 16, out);
    TextColor(WHITE, BLACK, out);
    draw();
    int nextX = hero.getX() + dx[hero.getSwordDirection()];
    int nextY = hero.getY() + dy[hero.getSwordDirection()];
    vector<int> creatureTIndex;
    bool findCreature = false;;
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && dungeons[Dungeon::dungeonIndex].creatures[i].getX() == hero.getX() && dungeons[Dungeon::dungeonIndex].creatures[i].getY() == hero.getY())
        {
            creatureTIndex.push_back(i);
            findCreature = true;
        }
    }
    while (!dungeons[Dungeon::dungeonIndex].isBoundary(Point(nextX, nextY)) && !dungeons[Dungeon::dungeonIndex].isObstacle(Point(nextX, nextY)) && !findCreature)
    {
        //Sleep(1);
        screen[hero.getY()][hero.getX()] = dungeons[Dungeon::dungeonIndex].getFloorSkin();
        hero.move(Point(dx[hero.getSwordDirection()], dy[hero.getSwordDirection()]));
        screen[hero.getY()][hero.getX()] = hero.getSkin();
        for (int i = 0; i < Creature::creatureTotal; i++)
        {
            if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && dungeons[Dungeon::dungeonIndex].creatures[i].getX() == hero.getX() && dungeons[Dungeon::dungeonIndex].creatures[i].getY() == hero.getY())
            {
                creatureTIndex.push_back(i);
                findCreature = true;
            }
        }
        nextX = hero.getX() + dx[hero.getSwordDirection()];
        nextY = hero.getY() + dy[hero.getSwordDirection()];
        printScreen();
    }
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (hero.getSwordDirection() == SNorth || hero.getSwordDirection() == SSouth)
        {
            if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && (dungeons[Dungeon::dungeonIndex].creatures[i].getX() == hero.getX() - 1 || dungeons[Dungeon::dungeonIndex].creatures[i].getX() == hero.getX() + 1) && dungeons[Dungeon::dungeonIndex].creatures[i].getY() == hero.getY())
            {
                creatureTIndex.push_back(i);
                screen[dungeons[Dungeon::dungeonIndex].creatures[i].getY()][dungeons[Dungeon::dungeonIndex].creatures[i].getX()] = '/';
            }
        }
        else
        {
            if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && dungeons[Dungeon::dungeonIndex].creatures[i].getX() == hero.getX() && (dungeons[Dungeon::dungeonIndex].creatures[i].getY() == hero.getY() + 1 || dungeons[Dungeon::dungeonIndex].creatures[i].getY() == hero.getY() - 1))
            {
                creatureTIndex.push_back(i);
                screen[dungeons[Dungeon::dungeonIndex].creatures[i].getY()][dungeons[Dungeon::dungeonIndex].creatures[i].getX()] = '/';
            }
        }
    }
    printScreen();
    for (int index : creatureTIndex)
    {
        hero.getExp(dungeons[Dungeon::dungeonIndex].creatures[index].hurt(100));
    }
    Sleep(1000);
    clearInputBuffer();
}

const void Chapter::DemonSlayer_BreathOfWater11()
{
    system("CLS");
    fontsize(128, 128, out);
    LoadFiles loadFiles;
    mciSendString(loadFiles.ConvertString("close mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("open \"extensions\\BreathOfWater11.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
    mciSendString(loadFiles.ConvertString("play mp3"), NULL, 0, NULL);
    TextColor(LIGHTCYAN, BLACK, out);
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
    fontsize(16, 16, out);
    TextColor(WHITE, BLACK, out);
    draw();
    Sleep(500);
    for (int i = 0; i < Creature::creatureTotal; i++)
    {
        if (dungeons[Dungeon::dungeonIndex].creatures[i].isLive() && dungeons[Dungeon::dungeonIndex].creatures[i].isAlert())
        {
            hero.getExp(dungeons[Dungeon::dungeonIndex].creatures[i].hurt(100));
            screen[dungeons[Dungeon::dungeonIndex].creatures[i].getY()][dungeons[Dungeon::dungeonIndex].creatures[i].getX()] = '/';
        }
    }
    printScreen();
    Sleep(1000);
    clearInputBuffer();
}