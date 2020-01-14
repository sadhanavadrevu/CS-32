#include "Actor.h"
#include "StudentWorld.h"


//Actor implementations

Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, Direction dir, int depth)
: GraphObject(imageID, startX, startY, dir, depth)
{
    m_world = world;
    m_alive = true;
}


Actor::~Actor() {}

void Actor::collideWithPit() {
    return;
}

void Actor::collideWithExit() {
    return;
}

void Actor::collideWithFlame() {
    setDead();
    return;
}

void Actor::collideWithGoodie(Goodie* g) {
    return;
}

void Actor::collideWithVomit() {
    return;
}

void Actor::collideWithLandmine(Landmine* mine) {
    return;
}

void Actor::setDead() {
    m_alive = false;
}

bool Actor::triggerZombieVomit() {
    return false;
}

bool Actor::triggersCitizens() {
    return false;
}

StudentWorld* Actor::getWorld() const {
    return m_world;
}

bool Actor::isAlive() const {
    return m_alive;
}

bool Actor::blocksMovement() {
    return false;
}

bool Actor::threatensCitizens() {
    return false;
}

bool Actor::blocksFlame() {
    return false;
}
//Environment implementations

Environment::Environment(StudentWorld* world, int imageID, double startX, double startY, int depth, Direction dir)
: Actor(world, imageID, startX, startY, dir, depth)
{}

Environment::~Environment() {};

//Moveable implementations
Moveable::Moveable(StudentWorld* world, int imageID, double startX, double startY)
: Actor(world, imageID, startX, startY) {
    m_ticksAlive = 0;
}

Moveable::~Moveable() {}

bool Moveable::blocksMovement() {
    return true;
}

void Moveable::collideWithPit() {
    setDead();
}

void Moveable::incTicks() {
    m_ticksAlive++;
}

bool Moveable::paralyzed() {
    if (m_ticksAlive % 2 == 0)
        return true;
    return false;
}

bool Moveable::triggersCitizens() {
    return true;
}

void Moveable::collideWithLandmine(Landmine* mine) {
    if (mine->isActive())
        mine->setDead();
}


//Human implementations
Human::Human(StudentWorld* world, int imageID, double startX, double startY)
: Moveable(world, imageID, startX, startY)
{
    m_infected = 0;
}

Human::~Human() {}

void Human::collideWithVomit() {
    setInfected();
}

void Human::setInfected() {
    m_infected++;
}

bool Human::triggerZombieVomit() {
    return true;
}

void Human::setUnInfected() {
    m_infected = 0;
}

int Human::getInfectionCount() {
    return m_infected;
}

//Citizen implementations
Citizen::Citizen(StudentWorld* world, double startX, double startY)
: Human(world, IID_CITIZEN, startX, startY) {}

Citizen::~Citizen() {}

void Citizen::setDead() {
    Actor::setDead();
    getWorld()->playSound(SOUND_CITIZEN_DIE);
    getWorld()->increaseScore(-1000);
    getWorld()->saveCitizen();
}

void Citizen::collideWithExit() {
    Actor::setDead();
    getWorld()->increaseScore(500);
    getWorld()->playSound(SOUND_CITIZEN_SAVED);
    getWorld()->saveCitizen();
}

void Citizen::doSomething() {
    if (!isAlive())
        return;
    incTicks();
    if (getInfectionCount() > 0) {
        setInfected();
        if (getInfectionCount() == 500) {
            setDead();
            getWorld()->playSound(SOUND_ZOMBIE_BORN);
            int zChance = randInt(1, 10);
            if (zChance <= 3) {
                SmartZombie* newSmartZombie = new SmartZombie(getWorld(), getX(), getY());
                getWorld()->addActor(newSmartZombie);
            }
            else {
                DumbZombie* newDumbZombie = new DumbZombie(getWorld(), getX(), getY());
                getWorld()->addActor(newDumbZombie);
            }
            return;
        }
    }
    if (paralyzed())
        return;
    bool threat;
    double closestX, closestY, closestDist = -1;
    getWorld()->locateNearestCitizenTrigger(getX(), getY(), closestX, closestY, closestDist, threat);
    if (threat == false && closestDist <= 80) {
        if (getX() == closestX) {
            if (getY() < closestY) {
                if (getWorld()->canMoveTo(this, getX(), getY() + 2)) {
                    setDirection(up);
                    moveTo(getX(), getY() + 2);
                    return;
                }
            }
            else if (getY() > closestY) {
                if (getWorld()->canMoveTo(this, getX(), getY() - 2)) {
                    setDirection(down);
                    moveTo(getX(), getY() - 2);
                    return;
                }
            }
        }
        else if (getY() == closestY) {
            if (getX() < closestX) {
                if (getWorld()->canMoveTo(this, getX() + 2, getY())) {
                    setDirection(right);
                    moveTo(getX() + 2, getY());
                    return;
                }
            }
            else if (getX() > closestX) {
                if (getWorld()->canMoveTo(this, getX() - 2, getY())) {
                    setDirection(left);
                    moveTo(getX() - 2, getY());
                    return;
                }
            }
        }
        else {
            int rand = randInt(1, 2);
            if (getX() < closestX && getY() > closestY) {
                switch (rand) {
                    case 1: { //move down first
                        if (getWorld()->canMoveTo(this, getX(), getY() - 2)) {
                            setDirection(down);
                            moveTo(getX(), getY() - 2);
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX() + 2, getY())) {
                                setDirection(right);
                                moveTo(getX() + 2, getY());
                                return;
                            }
                        }
                        break;
                    }
                    case 2: { //move right first
                        if (getWorld()->canMoveTo(this, getX() + 2, getY())) {
                            setDirection(right);
                            moveTo(getX() + 2, getY());
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX(), getY() - 2)) {
                                setDirection(down);
                                moveTo(getX(), getY() - 2);
                                return;
                            }
                        }
                        break;
                    }
                }
            }
            else if (getX() < closestX && getY() < closestY) {
                switch (rand) {
                    case 1: { //move up first
                        if (getWorld()->canMoveTo(this, getX(), getY() + 2)) {
                            setDirection(up);
                            moveTo(getX(), getY() + 2);
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX() + 2, getY())) {
                                setDirection(right);
                                moveTo(getX() + 2, getY());
                                return;
                            }
                        }
                        break;
                    }
                    case 2: { //move right first
                        if (getWorld()->canMoveTo(this, getX() + 2, getY())) {
                            setDirection(right);
                            moveTo(getX() + 2, getY());
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX(), getY() + 2)) {
                                setDirection(up);
                                moveTo(getX(), getY() + 2);
                                return;
                            }
                        }
                        break;
                    }
                }
            }
            else if (getX() > closestX && getY() < closestY) {
                switch (rand) {
                    case 1: { //move up first
                        if (getWorld()->canMoveTo(this, getX(), getY() + 2)) {
                            setDirection(up);
                            moveTo(getX(), getY() + 2);
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX() - 2, getY())) {
                                setDirection(left);
                                moveTo(getX() - 2, getY());
                                return;
                            }
                        }
                        break;
                    }
                    case 2: { //move left first
                        if (getWorld()->canMoveTo(this, getX() - 2, getY())) {
                            setDirection(left);
                            moveTo(getX() - 2, getY());
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX(), getY() + 2)) {
                                setDirection(up);
                                moveTo(getX(), getY() + 2);
                                return;
                            }
                        }
                        break;
                    }
                }
            }
            else if (getX() > closestX && getY() > closestY) {
                switch (rand) {
                    case 1: { //move down first
                        if (getWorld()->canMoveTo(this, getX(), getY() - 2)) {
                            setDirection(down);
                            moveTo(getX(), getY() - 2);
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX() - 2, getY())) {
                                setDirection(left);
                                moveTo(getX() - 2, getY());
                                return;
                            }
                        }
                        break;
                    }
                    case 2: { //move left first
                        if (getWorld()->canMoveTo(this, getX() - 2, getY())) {
                            setDirection(left);
                            moveTo(getX() - 2, getY());
                            return;
                        }
                        else {
                            if (getWorld()->canMoveTo(this, getX(), getY() - 2)) {
                                setDirection(down);
                                moveTo(getX(), getY() - 2);
                                return;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    else if (threat == true && closestDist <= 80) {
        double leftZDistance = -1, rightZDistance = -1, upZDistance = -1, downZDistance = -1;
        double currentZDistance = -1;
        double currentZy = -1, currentZx = -1;
        getWorld()->locateNearestCitizenThreat(getX(), getY(), currentZx, currentZy, currentZDistance);
        //left
        if (getWorld()->canMoveTo(this, getX() - 2, getY())) {
            double zombieX, zombieY = -1;
            getWorld()->locateNearestCitizenThreat(getX() - 2, getY(), zombieX, zombieY, leftZDistance);
        }
        //right
        if (getWorld()->canMoveTo(this, getX() + 2, getY())) {
            double zombieX, zombieY = -1;
            getWorld()->locateNearestCitizenThreat(getX() + 2, getY(), zombieX, zombieY, rightZDistance);
        }
        //down
        if (getWorld()->canMoveTo(this, getX(), getY() - 2)) {
            double zombieX, zombieY = -1;
            getWorld()->locateNearestCitizenThreat(getX(), getY() - 2, zombieX, zombieY, downZDistance);
        }
        //up
        if (getWorld()->canMoveTo(this, getX(), getY() + 2)) {
            double zombieX, zombieY = -1;
            getWorld()->locateNearestCitizenThreat(getX(), getY() + 2, zombieX, zombieY, upZDistance);
        }
        if (currentZDistance >= leftZDistance && currentZDistance >= rightZDistance && currentZDistance >= upZDistance && currentZDistance >= downZDistance)
            return;
        else if (leftZDistance >= rightZDistance && leftZDistance >= upZDistance && leftZDistance >= downZDistance) {
            setDirection(left);
            moveTo(getX() - 2, getY());
            return;
        }
        else if (rightZDistance >= leftZDistance && rightZDistance >= upZDistance && rightZDistance >= downZDistance) {
            setDirection(right);
            moveTo(getX() + 2, getY());
            return;
        }
        else if (upZDistance >= leftZDistance && upZDistance >= rightZDistance && upZDistance >= downZDistance) {
            setDirection(up);
            moveTo(getX(), getY() + 2);
            return;
        }
        else if (downZDistance >= leftZDistance && downZDistance >= rightZDistance && downZDistance >= upZDistance) {
            setDirection(down);
            moveTo(getX(), getY() - 2);
            return;
        }
    }
    else
        return;
}


void Citizen::collideWithVomit() {
    Human::collideWithVomit();
    if (getInfectionCount() == 1)
        getWorld()->playSound(SOUND_CITIZEN_INFECTED);
}

bool Citizen::triggersCitizens() {
    return false;
}


//Penelope implementations
Penelope::Penelope(StudentWorld* world, double startX, double startY)
: Human(world, IID_PLAYER, startX, startY)
{
    m_landmines = 0;
    m_charges = 0;
    m_vaccines = 0;
}

Penelope::~Penelope() {};

void Penelope::collideWithGoodie(Goodie* g) {
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(50);
    g->setDead();
    g->getPickedUp(this);
}

void Penelope::addLandmines() {
    m_landmines += 2;
}

void Penelope::addCharges() {
    m_charges += 5;
}

void Penelope::addVaccines() {
    m_vaccines += 1;
}

void Penelope::doSomething() {
    if (!isAlive())
        return;
    if(getInfectionCount() > 0) {
        setInfected();
        if (getInfectionCount() == 500) {
            setDead();
            return;
        }
    }
    int ch = -1;
    if (getWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_SPACE: {
                if (m_charges != 0) {
                    m_charges--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    switch (getDirection()) {
                        case right: {
                            for (int i = 1; i <= 3; i++) {
                                if (!getWorld()->isFlameBlockedAt(getX() + i*SPRITE_WIDTH, getY())) {
                                    Flame* newFlame = new Flame(getWorld(), getX() + i*SPRITE_WIDTH, getY(), right);
                                    getWorld()->addActor(newFlame);
                                }
                                else
                                    break;
                            }
                            break;
                        }
                        case left: {
                            for (int i = 1; i <= 3; i++) {
                                if (!getWorld()->isFlameBlockedAt(getX() - i*SPRITE_WIDTH, getY())) {
                                    Flame* newFlame = new Flame(getWorld(), getX() - i*SPRITE_WIDTH, getY(), left);
                                    getWorld()->addActor(newFlame);
                                }
                                else
                                    break;
                            }
                            break;
                        }
                        case up: {
                            for (int i = 1; i <= 3; i++) {
                                if (!getWorld()->isFlameBlockedAt(getX(), getY() + i*SPRITE_HEIGHT)) {
                                    Flame* newFlame = new Flame(getWorld(), getX(), getY() + i*SPRITE_HEIGHT, up);
                                    getWorld()->addActor(newFlame);
                                }
                                else
                                    break;
                            }
                            break;
                        }
                        case down: {
                            for (int i = 1; i <= 3; i++) {
                                if (!getWorld()->isFlameBlockedAt(getX(), getY() - i*SPRITE_HEIGHT)) {
                                    Flame* newFlame = new Flame(getWorld(), getX(), getY() - i*SPRITE_HEIGHT, down);
                                    getWorld()->addActor(newFlame);
                                }
                                else
                                    break;
                            }
                            break;
                        }
                            
                        default:
                            break;
                    }
                }
                break;
            }
            case KEY_PRESS_TAB: {
                if (m_landmines != 0) {
                    m_landmines--;
                    Landmine* newLandmine = new Landmine(getWorld(), getX(), getY());
                    getWorld()->addActor(newLandmine);
                }
                break;
            }
            case KEY_PRESS_ENTER: {
                if (m_vaccines != 0) {
                    m_vaccines--;
                    setUnInfected();
                }
                break;
            }
            case KEY_PRESS_LEFT: {
                setDirection(left);
                if (getWorld()->canMoveTo(this, getX() - 4, getY()))
                    moveTo(getX() - 4, getY());
                break;
            }
            case KEY_PRESS_RIGHT: {
                setDirection(right);
                if (getWorld()->canMoveTo(this, getX() + 4, getY()))
                    moveTo(getX() + 4, getY());
                break;
            }
            case KEY_PRESS_UP: {
                setDirection(up);
                if (getWorld()->canMoveTo(this, getX(), getY() + 4))
                    moveTo(getX(), getY() + 4);
                break;
            }
            case KEY_PRESS_DOWN: {
                setDirection(down);
                if (getWorld()->canMoveTo(this, getX(), getY() - 4))
                    moveTo(getX(), getY() - 4);
                break;
            }
            default:
                break;
        }
    }
}

void Penelope::collideWithExit() {
    if (getWorld()->noMoreCitizens()) {
        getWorld()->playSound(SOUND_LEVEL_FINISHED);
        getWorld()->finishLevel();
    }
}

int Penelope::getCharges() {
    return m_charges;
}

int Penelope::getVaccines() {
    return m_vaccines;
}

int Penelope::getLandmines() {
    return m_landmines;
}

void Penelope::setDead() {
    Actor::setDead();
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

//Zombie implementations
Zombie::Zombie(StudentWorld* world, double startX, double startY)
: Moveable(world, IID_ZOMBIE, startX, startY) {
    m_movementPlan = 0;
}

Zombie::~Zombie() {}

void Zombie::setDead() {
    Actor::setDead();
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
}

void Zombie::doSomething() {
    if (!isAlive())
        return;
    incTicks();
    if (paralyzed())
        return;
    double vomitX = 0, vomitY = 0;
    switch (getDirection()) {
        case right: {
            vomitX = getX() + SPRITE_WIDTH;
            vomitY = getY();
            break;
        }
        case left: {
            vomitX = getX() - SPRITE_WIDTH;
            vomitY = getY();
            break;
        }
        case up: {
            vomitX = getX();
            vomitY = getY() + SPRITE_HEIGHT;
            break;
        }
        case down: {
            vomitX = getX();
            vomitY = getY() - SPRITE_HEIGHT;
            break;
        }
        default:
            break;
    }
    Actor* collide = getWorld()->collision(this, vomitX, vomitY);
    if (collide != nullptr && collide->triggerZombieVomit()) {
        int vomitChance = randInt(1, 3);
        if (vomitChance == 1) {
            Vomit* newVomit = new Vomit(getWorld(), vomitX, vomitY, getDirection());
            getWorld()->addActor(newVomit);
            getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
            return;
        }
    }
    if (m_movementPlan == 0) {
        createNewMovementPlan();
    }
    double destX = 0, destY = 0;
    switch (getDirection()) {
        case right:
            destX = getX() + 1;
            destY = getY();
            break;
        case left:
            destX = getX() - 1;
            destY = getY();
            break;
        case up:
            destX = getX();
            destY = getY() + 1;
            break;
        case down:
            destX = getX();
            destY = getY() - 1;
            break;
    }
    if (getWorld()->canMoveTo(this, destX, destY)) {
        moveTo(destX, destY);
        m_movementPlan--;
    }
    else
        m_movementPlan = 0;
}

bool Zombie::threatensCitizens() {
    return true;
}

void Zombie::setNewMovementPlan(int dist, Direction dir) {
    m_movementPlan = dist;
    setDirection(dir);
}

//DumbZombie implementations
DumbZombie::DumbZombie(StudentWorld* world, double startX, double startY)
: Zombie(world, startX, startY) {}

DumbZombie::~DumbZombie() {}

void DumbZombie::setDead() {
    Zombie::setDead();
    getWorld()->increaseScore(1000);
    int vaccineChance = randInt(1, 10);
    if (vaccineChance == 1) {
        int vaccDir = randInt(1, 4);
        switch (vaccDir) {
            case 1: { //fling to right
                if (getWorld()->collision(this, getX() + SPRITE_WIDTH, getY()) == nullptr) {
                    VaccineGoodie* newVaccine = new VaccineGoodie(getWorld(), getX() + SPRITE_WIDTH, getY());
                    getWorld()->addActor(newVaccine);
                }
                break;
            }
            case 2: { //fling to left
                if (getWorld()->collision(this, getX() - SPRITE_WIDTH, getY()) == nullptr) {
                    VaccineGoodie* newVaccine = new VaccineGoodie(getWorld(), getX() - SPRITE_WIDTH, getY());
                    getWorld()->addActor(newVaccine);
                }
                break;
            }
            case 3: { //fling up
                if (getWorld()->collision(this, getX(), getY() + SPRITE_HEIGHT) == nullptr) {
                    VaccineGoodie* newVaccine = new VaccineGoodie(getWorld(), getX(), getY() + SPRITE_HEIGHT);
                    getWorld()->addActor(newVaccine);
                }
                break;
            }
            case 4: { //fling down
                if (getWorld()->collision(this, getX(), getY() - SPRITE_HEIGHT) == nullptr) {
                    VaccineGoodie* newVaccine = new VaccineGoodie(getWorld(), getX(), getY() - SPRITE_HEIGHT);
                    getWorld()->addActor(newVaccine);
                }
                break;
            }
        }
    }
}

void DumbZombie::createNewMovementPlan() {
    int dist = randInt(3, 10);
    switch (randInt(1, 4)) {
        case 1:
            setNewMovementPlan(dist, right);
            break;
        case 2:
            setNewMovementPlan(dist, left);
            break;
        case 3:
            setNewMovementPlan(dist, up);
            break;
        case 4:
            setNewMovementPlan(dist, down);
            break;
    }
}

//SmartZombie implementations
SmartZombie::SmartZombie(StudentWorld* world, double startX, double startY)
: Zombie(world, startX, startY) {}

SmartZombie::~SmartZombie() {}

void SmartZombie::setDead() {
    Zombie::setDead();
    getWorld()->increaseScore(1000);
}

void SmartZombie::createNewMovementPlan() {
    int dist = randInt(3, 10);
    double closestX = 0;
    double closestY = 0;
    double distance = 0;
    if (getWorld()->locateNearestVomitTrigger(getX(), getY(),  closestX, closestY, distance)) {
        if (distance > 80) {
            switch (randInt(1, 4)) {
                case 1:
                    setNewMovementPlan(dist, right);
                    break;
                case 2:
                    setNewMovementPlan(dist, left);
                    break;
                case 3:
                    setNewMovementPlan(dist, up);
                    break;
                case 4:
                    setNewMovementPlan(dist, down);
                    break;
            }
        }
        else {
            if (getX() == closestX) {
                if (getY() < closestY)
                    setNewMovementPlan(dist, up);
                else if (getY() > closestY)
                    setNewMovementPlan(dist, down);
            }
            else if (getY() == closestY) {
                if (getX() < closestX)
                    setNewMovementPlan(dist, right);
                else if (getX() > closestX)
                    setNewMovementPlan(dist, left);
            }
            else {
                int rand = randInt(1, 2);
                if (getX() < closestX && getY() > closestY) {
                    switch (rand) {
                        case 1:
                            setNewMovementPlan(dist, down);
                            break;
                        case 2:
                            setNewMovementPlan(dist, right);
                            break;
                    }
                }
                else if (getX() < closestX && getY() < closestY) {
                    switch (rand) {
                        case 1:
                            setNewMovementPlan(dist, up);
                            break;
                        case 2:
                            setNewMovementPlan(dist, right);
                            break;
                    }
                }
                else if (getX() > closestX && getY() < closestY) {
                    switch (rand) {
                        case 1:
                            setNewMovementPlan(dist, up);
                            break;
                        case 2:
                            setNewMovementPlan(dist, left);
                            break;
                    }
                }
                else if (getX() > closestX && getY() > closestY) {
                    switch (rand) {
                        case 1:
                            setNewMovementPlan(dist, down);
                            break;
                        case 2:
                            setNewMovementPlan(dist, left);
                            break;
                    }
                }
            }
        }
        
    }
}

//Inflammable implementations
Inflammable::Inflammable(StudentWorld* world, int imageID, double startX, double startY, int depth)
: Environment(world, imageID, startX, startY, depth) {}

Inflammable::~Inflammable() {}

void Inflammable::collideWithFlame() {
    return;
}

//Wall implementations
Wall::Wall(StudentWorld* world, double startX, double startY)
: Inflammable(world, IID_WALL, startX, startY)
{}

Wall::~Wall() {};

void Wall::doSomething() {};

bool Wall::blocksMovement() {
    return true;
}

bool Wall::blocksFlame() {
    return true;
}

//Pit implementations
Pit::Pit(StudentWorld* world, double startX, double startY)
: Inflammable(world, IID_PIT, startX, startY) {}

Pit::~Pit() {}

void Pit::doSomething() {
    Actor* collidedWith = getWorld()->collision(this, getX(), getY());
    if(collidedWith != nullptr) {
        collidedWith->collideWithPit();
    }
}

//Exit implementations
Exit::Exit(StudentWorld* world, double startX, double startY)
: Inflammable(world, IID_EXIT, startX, startY, 1) {}

Exit::~Exit() {}

void Exit::doSomething() {
    Actor* collidedWith = getWorld()->collision(this, getX(), getY());
    if (collidedWith != nullptr) {
        collidedWith->collideWithExit();
    }
}

bool Exit::blocksFlame() {
    return true;
}

//Goodie implementations
Goodie::Goodie(StudentWorld* world, int imageID, double startX, double startY)
: Environment(world, imageID, startX, startY, 1) {}

Goodie::~Goodie() {}

void Goodie::doSomething() {
    Actor* collidedWith = getWorld()->collision(this, getX(), getY());
    if(collidedWith != nullptr) {
        collidedWith->collideWithGoodie(this);
    }
}

//VaccineGoodie implementations
VaccineGoodie::VaccineGoodie(StudentWorld* world, double startX, double startY)
: Goodie(world, IID_VACCINE_GOODIE, startX, startY) {}

VaccineGoodie::~VaccineGoodie() {}

void VaccineGoodie::getPickedUp(Penelope *p) {
    p->addVaccines();
}

//LandmineGoodie implementations
LandmineGoodie::LandmineGoodie(StudentWorld* world, double startX, double startY)
: Goodie(world, IID_LANDMINE_GOODIE, startX, startY) {}

LandmineGoodie::~LandmineGoodie() {}

void LandmineGoodie::getPickedUp(Penelope *p) {
    p->addLandmines();
}

//GasCanGoodie implementations
GasCanGoodie::GasCanGoodie(StudentWorld* world, double startX, double startY)
: Goodie(world, IID_GAS_CAN_GOODIE, startX, startY) {}

GasCanGoodie::~GasCanGoodie() {}

void GasCanGoodie::getPickedUp(Penelope *p) {
    p->addCharges();
}

//Projectile implementations
Projectile::Projectile(StudentWorld* world, int imageID, double startX, double startY, Direction dir)
: Environment(world, imageID, startX, startY, dir) {
    aliveTime = 2;
}

Projectile::~Projectile() {}

void Projectile::decreaseAliveTime() {
    aliveTime--;
}

int Projectile::getAliveTime() {
    return aliveTime;
}

//Flame implementations
Flame::Flame(StudentWorld* world, double startX, double startY, Direction dir)
: Projectile(world, IID_FLAME, startX, startY, dir) {}

Flame::~Flame() {}

void Flame::doSomething() {
    if (!isAlive())
        return;
    decreaseAliveTime();
    if (getAliveTime() == 0)
        setDead();
    Actor* collidedWith = getWorld()->collision(this, getX(), getY());
    if (collidedWith != nullptr) {
        collidedWith->collideWithFlame();
    }
}

void Flame::collideWithLandmine(Landmine* mine) {
    mine->setDead();
}

//Vomit implementations
Vomit::Vomit(StudentWorld* world, double startX, double startY, Direction dir)
: Projectile(world, IID_VOMIT, startX, startY, dir) {}

Vomit::~Vomit() {}

void Vomit::doSomething() {
    if (!isAlive())
        return;
    decreaseAliveTime();
    if (getAliveTime() == 0)
        setDead();
    Actor* collidedWith = getWorld()->collision(this, getX(), getY());
    if (collidedWith != nullptr) {
        collidedWith->collideWithVomit();
    }
}

//Landmine implementations
Landmine::Landmine(StudentWorld* world, double startX, double startY)
: Environment(world, IID_LANDMINE, startX, startY, 1) {
    m_active = false;
    m_safetyTicks = 30;
}

Landmine::~Landmine() {}

void Landmine::doSomething() {
    if (!isAlive())
        return;
    if (m_active == false) {
        m_safetyTicks--;
        if (m_safetyTicks == 0)
            m_active = true;
        return;
    }
    Actor* collidedWith = getWorld()->collision(this, getX(), getY());
    if (collidedWith != nullptr) {
        collidedWith->collideWithLandmine(this);
    }
}
        
bool Landmine::isActive(){
    return m_active;
}

void Landmine::setDead() {
    Actor::setDead();
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    Flame* flameAtLocation = new Flame(getWorld(), getX(), getY(), up);
    getWorld()->addActor(flameAtLocation);
    Pit* pitAtLocation = new Pit(getWorld(), getX(), getY());
    getWorld()->addActor(pitAtLocation);
    Flame* westFlame = new Flame(getWorld(), getX() - SPRITE_WIDTH, getY(), up);
    getWorld()->addActor(westFlame);
    Flame* eastFlame = new Flame(getWorld(), getX() + SPRITE_WIDTH, getY(), up);
    getWorld()->addActor(eastFlame);
    Flame* southwestFlame = new Flame(getWorld(), getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
    getWorld()->addActor(southwestFlame);
    Flame* northwestFlame = new Flame(getWorld(), getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
    getWorld()->addActor(northwestFlame);
    Flame* northeastFlame = new Flame(getWorld(), getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
    getWorld()->addActor(northeastFlame);
    Flame* southeastFlame = new Flame(getWorld(), getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
    getWorld()->addActor(southeastFlame);
    Flame* northFlame = new Flame(getWorld(), getX(), getY() + SPRITE_HEIGHT, up);
    getWorld()->addActor(northFlame);
    Flame* southFlame = new Flame(getWorld(), getX(), getY() - SPRITE_HEIGHT, up);
    getWorld()->addActor(southFlame);
}


