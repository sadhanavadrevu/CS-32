//
//  Game.hpp
//  Project1
//
//  Created by Sadhana Vadrevu on 1/10/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <iostream>

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};

#endif /* Game_h */


