//
//  main.cpp
//  hw2p3
//
//  Created by Sadhana Vadrevu on 1/31/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include <iostream>
#include <queue>
using namespace std;

class Coord;

void markDiscovered(string maze[], Coord& cur);
void moveSouth(string maze[], queue<Coord>& cStack, Coord& cur);
void moveNorth(string maze[], queue<Coord>& cStack, Coord& cur);
void moveEast(string maze[], queue<Coord>& cStack, Coord& cur);
void moveWest(string maze[], queue<Coord>& cStack, Coord& cur);
bool canMove(string maze[], Coord cur);

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    queue<Coord> coordQueue;
    
    if (sr < 0 || sr == nRows || sc < 0 || sc == nCols)
        return false;
    if (er < 0 || er == nRows || ec < 0 || ec == nCols)
        return false;
    
    Coord start(sr, sc);
    coordQueue.push(start);
    markDiscovered(maze, start);
    
    while (!coordQueue.empty()) {
        Coord here = coordQueue.front();
        coordQueue.pop();
        if (here.r() == er && here.c() == ec)
            return true;
        moveSouth(maze, coordQueue, here);
        moveWest(maze, coordQueue, here);
        moveNorth(maze, coordQueue, here);
        moveEast(maze, coordQueue, here);
    }
    return false;
}

//helper function implementations
void markDiscovered(string maze[], Coord& cur) {
    maze[cur.r()][cur.c()] = '!';
}

void moveSouth(string maze[], queue<Coord>& cQueue, Coord& cur) {
    Coord newCur(cur.r() + 1, cur.c());
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cQueue.push(newCur);
    }
}

void moveNorth(string maze[], queue<Coord>& cQueue, Coord& cur) {
    Coord newCur(cur.r() - 1, cur.c());
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cQueue.push(newCur);
    }
}

void moveEast(string maze[], queue<Coord>& cQueue, Coord& cur) {
    Coord newCur(cur.r(), cur.c() + 1);
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cQueue.push(newCur);
    }
}

void moveWest(string maze[], queue<Coord>& cQueue, Coord& cur) {
    Coord newCur(cur.r(), cur.c() - 1);
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cQueue.push(newCur);
    }
}

bool canMove(string maze[], Coord cur) {
    if(maze[cur.r()][cur.c()] == 'X' || maze[cur.r()][cur.c()] == '!')
        return false;
    return true;
}
