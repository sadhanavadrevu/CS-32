#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "SoundFX.h"

class StudentWorld;
class Goodie;
class Penelope;
class Landmine;

class Actor: public GraphObject {
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir = right, int depth = 0);
    virtual ~Actor();
    
    virtual void doSomething() = 0;
    
    virtual void collideWithPit(); //for humans and zombies, set dead, everything else =  nothing
    virtual void collideWithExit(); //for citizens, function saves them and flames get blocked; everyone else, this function has no functionality
    virtual void collideWithGoodie(Goodie* g); //for Penelope, pick up, everything else = nothing
    virtual void collideWithLandmine(Landmine* mine); //for penelope and citizen, get triggered if landmine is active, for flame, always trigger
    virtual void collideWithVomit(); //for humans, get infected
    virtual void collideWithFlame();
    virtual bool triggerZombieVomit();
    virtual bool triggersCitizens();
    virtual bool threatensCitizens();
    virtual bool blocksMovement();
    virtual bool blocksFlame();
    virtual void setDead();
    
    StudentWorld* getWorld() const;
    bool isAlive() const;
    
private:
    StudentWorld* m_world;
    bool m_alive; //set to true if alive, false if dead
};

class Moveable: public Actor {
public:
    Moveable(StudentWorld* world, int imageID, double startX, double startY);
    virtual ~Moveable();
    
    //specialized functions
    bool blocksMovement();
    void collideWithPit();
    bool triggersCitizens();
    void collideWithLandmine(Landmine* mine);
    
    //unique Moveable functions
    void incTicks();
    bool paralyzed();
    
private:
    int m_ticksAlive;
};

class Human: public Moveable {
public:
    Human(StudentWorld* world, int imageID, double startX, double startY);
    virtual ~Human();
    
    //specialized functions
    virtual void collideWithVomit();
    bool triggerZombieVomit();
    
    //unique Human functions
    void setInfected();
    void setUnInfected();
    int getInfectionCount();
    
private:
    int m_infected;
};

class Citizen: public Human {
public:
    Citizen(StudentWorld* world, double startX, double startY);
    ~Citizen();
    
    //specialized functions
    void setDead();
    void collideWithExit();
    void doSomething();
    void collideWithVomit();
    bool triggersCitizens();
    
};

class Penelope: public Human {
public:
    Penelope(StudentWorld* world, double startX, double startY);
    ~Penelope();
    
    //specialized functions
    void collideWithGoodie(Goodie* g);
    void collideWithExit();
    void setDead();
    void doSomething();
    
    //unique Penelope functions
    void addLandmines();
    void addCharges();
    void addVaccines();
    int getLandmines();
    int getCharges();
    int getVaccines();
    
private:
    int m_landmines;
    int m_charges;
    int m_vaccines;
};

class Zombie: public Moveable {
public:
    Zombie(StudentWorld* world, double startX, double startY);
    virtual ~Zombie();
    
    //specialized functions
    void doSomething();
    virtual void setDead();
    bool threatensCitizens();
    
    //unique Zombie functions
    void setNewMovementPlan(int dist, Direction dir);
    
private:
    virtual void createNewMovementPlan() = 0;
    
private:
    int m_movementPlan;
    int m_ticksAlive;
};

class DumbZombie: public Zombie {
public:
    DumbZombie(StudentWorld* world, double startX, double startY);
    ~DumbZombie();
    
    //specialized functions
    void setDead();
    
private:
    void createNewMovementPlan();
};

class SmartZombie: public Zombie {
public:
    SmartZombie(StudentWorld* world, double startX, double startY);
    ~SmartZombie();
    
    //specialized functions
    void setDead();
    
private:
    void createNewMovementPlan();
};

class Environment: public Actor {
public:
    Environment(StudentWorld* world, int imageID, double startX, double startY, int depth = 0, Direction dir = right);
    virtual ~Environment();
    
};

class Inflammable: public Environment {
public:
    Inflammable(StudentWorld* world, int imageID, double startX, double startY, int depth = 0);
    virtual ~Inflammable();
    
    //specialized functions
    void collideWithFlame();
};

class Wall: public Inflammable {
public:
    Wall(StudentWorld* world, double startX, double startY);
    ~Wall();
    
    //specialized functions
    void doSomething();
    bool blocksMovement();
    bool blocksFlame();
};

class Pit: public Inflammable {
public:
    Pit(StudentWorld* world, double startX, double startY);
    ~Pit();
    
    void doSomething();
};

class Exit: public Inflammable {
public:
    Exit(StudentWorld* world, double startX, double startY);
    ~Exit();
    
    void doSomething();
    bool blocksFlame();
};

class Goodie: public Environment {
public:
    Goodie(StudentWorld* world, int imageID, double startX, double startY);
    virtual ~Goodie();
    
    //specialized function
    void doSomething();
    
    virtual void getPickedUp(Penelope* p) = 0;
};

class VaccineGoodie: public Goodie {
public:
    VaccineGoodie(StudentWorld* world, double startX, double startY);
    ~VaccineGoodie();
    
    void getPickedUp(Penelope* p);
};

class LandmineGoodie: public Goodie {
public:
    LandmineGoodie(StudentWorld* world, double startX, double startY);
    ~LandmineGoodie();

    void getPickedUp(Penelope* p);
};

class GasCanGoodie: public Goodie {
public:
    GasCanGoodie(StudentWorld* world, double startX, double startY);
    ~GasCanGoodie();
    
    void getPickedUp(Penelope* p);
};

class Projectile: public Environment {
public:
    Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction dir);
    virtual ~Projectile();
    
    void decreaseAliveTime();
    int getAliveTime();
    
private:
    int aliveTime;
};

class Flame: public Projectile {
public:
    Flame(StudentWorld* world, double startX, double startY, Direction dir);
    ~Flame();
    
    void doSomething();
    void collideWithLandmine(Landmine* mine);
};

class Vomit: public Projectile {
public:
    Vomit(StudentWorld* world, double startX, double startY, Direction dir);
    ~Vomit();
    
    void doSomething();
};

class Landmine: public Environment {
public:
    Landmine(StudentWorld* world, double startX, double startY);
    ~Landmine();
    
    void doSomething();
    void setDead();
    bool isActive();
    
private:
    bool m_active;
    int m_safetyTicks;
};

#endif // ACTOR_H_
