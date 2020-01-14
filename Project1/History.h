//
//  History.h
//  Project1
//
//  Created by Sadhana Vadrevu on 1/14/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#ifndef History_h
#define History_h

#include <stdio.h>
#include "globals.h"
#include <iostream>

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    
private:
    char m_grid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
};

#endif /* History_h */
