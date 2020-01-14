//
//  Arena.h
//  Project1
//
//  Created by Sadhana Vadrevu on 1/10/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#ifndef Arena_h
#define Arena_h

#include <stdio.h>
#include "globals.h"
#include "History.h"
#include <string>
#include <iostream>

class Zombie;
class Player;

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     zombieCount() const;
    int     numZombiesAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    History& history();
    
    // Mutators
    bool   addZombie(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackZombieAt(int r, int c, int dir);
    bool   moveZombies();
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Zombie* m_zombies[MAXZOMBIES];
    int     m_nZombies;
    History m_history;
};

#endif /* Arena_h */


