#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_levelFinished = false;
    m_citizens = 0;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

void StudentWorld::finishLevel() {
    m_levelFinished = true;
}

int StudentWorld::loadLevel() {
    int lev = getLevel();
    Level level(assetPath());
    ostringstream levFile;
    
    levFile << "level0" << lev << ".txt";
    string levelFile = levFile.str();
    Level::LoadResult result = level.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found || lev == 100) {
        cerr << "Could not find level0" << lev << ".txt data file" << endl;
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level " << lev << endl;
        Level::MazeEntry ge;
        for (int i = 0; i < VIEW_WIDTH; i++) {
            for (int j = 0; j < VIEW_HEIGHT; j++) {
                ge = level.getContentsOf(i, j);
                switch (ge) {
                    case Level::empty: {
                        break;
                    }
                    case Level::wall: {
                        Wall* newWall = new Wall(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newWall);
                        break;
                    }
                    case Level::player: {
                        m_player = new Penelope(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(m_player);
                        break;
                    }
                    case Level::pit: {
                        Pit* newPit = new Pit(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newPit);
                        break;
                    }
                    case Level::exit: {
                        Exit* newExit = new Exit(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newExit);
                        break;
                    }
                    case Level::vaccine_goodie: {
                        VaccineGoodie* newVGoodie = new VaccineGoodie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newVGoodie);
                        break;
                    }
                    case Level::gas_can_goodie: {
                        GasCanGoodie* newGCGoodie = new GasCanGoodie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newGCGoodie);
                        break;
                    }
                    case Level::landmine_goodie: {
                        LandmineGoodie* newLGoodie = new LandmineGoodie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newLGoodie);
                        break;
                    }
                    case Level::smart_zombie: {
                        SmartZombie* newSmartZombie = new SmartZombie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newSmartZombie);
                        break;
                    }
                    case Level::dumb_zombie: {
                        DumbZombie* newDumbZombie = new DumbZombie(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newDumbZombie);
                        break;
                    }
                        
                    case Level::citizen: {
                        Citizen* newCitizen = new Citizen(this, SPRITE_WIDTH * i, SPRITE_HEIGHT * j);
                        m_actors.push_back(newCitizen);
                        m_citizens++;
                        break;
                    }
                        
                    default:
                        break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isFlameBlockedAt(double x, double y) const {
    vector<Actor*>::const_iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        Actor* collidedWith = collision(*it, x, y);
        if (collidedWith != nullptr && collidedWith->blocksFlame())
            return true;
        it++;
    }
    return false;
}

string StudentWorld::gameStat(int score, int level, int lives, int vacc, int flames, int mines, int infected) {
    ostringstream stat;
    ostringstream scoreoss;
    scoreoss.fill('0');
    if (score < 0)
        scoreoss << "-" << setw(6) << -1*score;
    else
        scoreoss << setw(6) << score;
    string scorestr = scoreoss.str();
    stat << "Score: " << scorestr << "  Level: " << level << "  Lives: " << lives << "  Vaccines: " << vacc << "  Flames: " << flames << "  Mines: " << mines << "  Infected: " << infected;
    string statstr = stat.str();
    return statstr;
}

void StudentWorld::addActor(Actor* actor) {
    m_actors.push_back(actor);
}

int StudentWorld::init()
{
    m_levelFinished = false;
    m_citizens = 0;
    int levelStatus = loadLevel();
    string stat = gameStat(getScore(), getLevel(), getLives(), m_player->getVaccines(), m_player->getCharges(), m_player->getLandmines(), m_player->getInfectionCount());
    this->setGameStatText(stat);
    return levelStatus;
}

bool StudentWorld::noMoreCitizens() {
    if (m_citizens == 0)
        return true;
    else
        return false;
}

int StudentWorld::move()
{
    string stat = gameStat(getScore(), getLevel(), getLives(), m_player->getVaccines(), m_player->getCharges(), m_player->getLandmines(), m_player->getInfectionCount());
    setGameStatText(stat);
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        (*it)->doSomething();
        if (!m_player->isAlive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_levelFinished) {
            cout << "level: " << getLevel() << endl;
            return GWSTATUS_FINISHED_LEVEL;
        }
        if (!(*it)->isAlive()) {
            delete *it;
            it = m_actors.erase(it);
            continue;
        }
        it++;
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        delete *it;
        it = m_actors.erase(it);
    }
}

bool StudentWorld::canMoveTo(Actor* me, double destX, double destY) {
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if ((*it)->blocksMovement() && (*it) != me) {
            double itXBoundary1 = (*it)->getX()-8 + 1;
            double itXBoundary2 = (*it)->getX()+8 - 1 ;
            double itYBoundary1 = (*it)->getY()-8 + 1;
            double itYBoundary2 = (*it)->getY()+8 - 1;
            double actXBoundary1 = destX-8 + 1;
            double actXBoundary2 = destX+8 - 1;
            double actYBoundary1 = destY-8 + 1;
            double actYBoundary2 = destY+8 - 1;
            if (itXBoundary1 <= actXBoundary2 && itXBoundary2 >= actXBoundary1 && itYBoundary1 <= actYBoundary2 && itYBoundary2 >= actYBoundary1) //check for NOT conditions of no overlap
                return false;
        }
        it++;
    }
    return true;
}

bool StudentWorld::locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance) {
    vector<Actor*>::const_iterator it;
    it = m_actors.begin();
    double closestDist = 300;
    double closestX = 0;
    double closestY = 0;
    while (it != m_actors.end()) {
        if ((*it)->triggerZombieVomit()) {
            double humanX = (*it)->getX();
            double humanY = (*it)->getY();
            double dist2 = (humanX - x)*(humanX - x) + (humanY - y)*(humanY - y);
            double dist = sqrt(dist2);
            if (dist < closestDist) {
                closestX = humanX;
                closestY = humanY;
                closestDist = dist;
            }
        }
        it++;
    }
    if (closestDist == 300)
        return false;
    else {
        otherX = closestX;
        otherY = closestY;
        distance = closestDist;
        return true;
    }
}

bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double &otherX, double &otherY, double &distance, bool &isThreat) const {
    vector<Actor*>::const_iterator it;
    it = m_actors.begin();
    double closestDist = 300;
    double closestX = 0;
    double closestY = 0;
    while (it != m_actors.end()) {
        if ((*it)->triggersCitizens()) {
            double humanX = (*it)->getX();
            double humanY = (*it)->getY();
            double dist2 = (humanX - x)*(humanX - x) + (humanY - y)*(humanY - y);
            double dist = sqrt(dist2);
            if (dist < closestDist) {
                if ((*it)->threatensCitizens())
                    isThreat = true;
                else
                    isThreat = false;
                closestX = humanX;
                closestY = humanY;
                closestDist = dist;
            }
        }
        it++;
    }
    if (closestDist == 300)
        return false;
    else {
        otherX = closestX;
        otherY = closestY;
        distance = closestDist;
        return true;
    }
}

bool StudentWorld::locateNearestCitizenThreat(double x, double y, double &otherX, double &otherY, double &distance) const {
    vector<Actor*>::const_iterator it;
    it = m_actors.begin();
    double closestDist = 300;
    double closestX = 0;
    double closestY = 0;
    while (it != m_actors.end()) {
        if ((*it)->threatensCitizens()) {
            double humanX = (*it)->getX();
            double humanY = (*it)->getY();
            double dist2 = (humanX - x)*(humanX - x) + (humanY - y)*(humanY - y);
            double dist = sqrt(dist2);
            if (dist < closestDist) {
                closestX = humanX;
                closestY = humanY;
                closestDist = dist;
            }
        }
        it++;
    }
    if (closestDist == 300)
        return false;
    else {
        otherX = closestX;
        otherY = closestY;
        distance = closestDist;
        return true;
    }
}

Actor* StudentWorld::collision(Actor* me, double X, double Y) const {
    vector<Actor*>::const_iterator it;
    it = m_actors.begin();
    while (it != m_actors.end()) {
        if (*it != me) {
            double dX = X - (*it)->getX();
            double dY = Y - (*it)->getY();
            double dist = (dX*dX) + (dY*dY);
            if (dist <= 10*10) {
                return *it;
            }
        }
        it++;
    }
    return nullptr;
}

void StudentWorld::saveCitizen() {
    m_citizens--;
}
