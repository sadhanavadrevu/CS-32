//
//  main.cpp
//  hw2p1
//
//  Created by Sadhana Vadrevu on 1/31/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include <iostream>
#include <stack>
using namespace std;

class Coord;

//helper functions
void markDiscovered(string maze[], Coord& cur);
void moveSouth(string maze[], stack<Coord>& cStack, Coord& cur);
void moveNorth(string maze[], stack<Coord>& cStack, Coord& cur);
void moveEast(string maze[], stack<Coord>& cStack, Coord& cur);
void moveWest(string maze[], stack<Coord>& cStack, Coord& cur);
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
    stack<Coord> coordStack;
    
    if (sr < 0 || sr == nRows || sc < 0 || sc == nCols)
        return false;
    if (er < 0 || er == nRows || ec < 0 || ec == nCols)
        return false;
    
    
    Coord start(sr, sc);
    coordStack.push(start);
    markDiscovered(maze, start);
    
    while (!coordStack.empty()) {
        Coord here = coordStack.top();
        coordStack.pop();
        if (here.r() == er && here.c() == ec)
            return true;
        moveSouth(maze, coordStack, here);
        moveWest(maze, coordStack, here);
        moveNorth(maze, coordStack, here);
        moveEast(maze, coordStack, here);
    }
    return false;
}

//helper function implementations
void markDiscovered(string maze[], Coord& cur) {
    maze[cur.r()][cur.c()] = '!';
}

void moveSouth(string maze[], stack<Coord>& cStack, Coord& cur) {
    Coord newCur(cur.r() + 1, cur.c());
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cStack.push(newCur);
    }
}

void moveNorth(string maze[], stack<Coord>& cStack, Coord& cur) {
    Coord newCur(cur.r() - 1, cur.c());
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cStack.push(newCur);
    }
}

void moveEast(string maze[], stack<Coord>& cStack, Coord& cur) {
    Coord newCur(cur.r(), cur.c() + 1);
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cStack.push(newCur);
    }
}

void moveWest(string maze[], stack<Coord>& cStack, Coord& cur) {
    Coord newCur(cur.r(), cur.c() - 1);
    if (canMove(maze, newCur)) {
        markDiscovered(maze, newCur);
        cStack.push(newCur);
    }
}

bool canMove(string maze[], Coord cur) {
    if(maze[cur.r()][cur.c()] == 'X' || maze[cur.r()][cur.c()] == '!')
        return false;
    return true;
}

