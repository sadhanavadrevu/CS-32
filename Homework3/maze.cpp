//
//  maze.cpp
//  hw3p3
//
//  Created by Sadhana Vadrevu on 2/11/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    
    //check for proper input
    if (sr < 0 || sr >= nRows || sc < 0 || sc >= nCols)
        return false;
    if (er < 0 || er >= nRows || ec < 0 || ec >= nCols )
        return false;
    
    //if the starting position is the ending position, maze has been solved
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '!'; //mark the starting position as discovered
    if (maze[sr + 1][sc] == '.') //move south if the spot is open
        if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)) 
            return true;
    if (maze[sr - 1][sc] == '.') //move north if the spot is open
        if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
            return true;
    if (maze[sr][sc + 1] == '.') //move east if the spot is open
        if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    if (maze[sr][sc - 1] == '.') //move west if the spot is open
        if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    return false;
}
