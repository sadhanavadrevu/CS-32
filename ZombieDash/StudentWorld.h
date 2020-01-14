#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    bool canMoveTo(Actor* me, double destX, double destY);
    Actor* collision(Actor* me, double X, double Y) const;
    void addActor(Actor* actor);
    bool isFlameBlockedAt(double x, double y) const;
    bool noMoreCitizens();
    void saveCitizen();
    bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);
    bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const;
    bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const;
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void finishLevel();

private:
    int loadLevel();
    std::string gameStat(int score, int level, int lives, int vacc, int flames, int mines, int infected);
    std::vector<Actor*> m_actors;
    Penelope* m_player;
    bool m_levelFinished;
    int m_citizens;
};

#endif // STUDENTWORLD_H_
