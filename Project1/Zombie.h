//
//  Zombie.h
//  Project1
//
//  Created by Sadhana Vadrevu on 1/10/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#ifndef Zombie_h
#define Zombie_h

#include <iostream>
#include <cstdlib>

class Arena;

class Zombie
{
public:
    // Constructor
    Zombie(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool getAttacked(int dir);
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
};


#endif /* Zombie_h */




