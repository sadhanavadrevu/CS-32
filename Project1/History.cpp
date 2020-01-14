//
//  History.cpp
//  Project1
//
//  Created by Sadhana Vadrevu on 1/14/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include "History.h"
using namespace std;

History::History(int nRows, int nCols) {
    m_rows = nRows;
    m_cols = nCols;
    
    for (int i = 1; i <= m_rows; i++) {
        for (int j = 1; j <= m_cols; j++) {
            m_grid[i][j] = '.';
        }
    }
}

bool History::record(int r, int c) {
    if (r <= 0 || r > m_rows || c <= 0 || c > m_cols)
        return false;
    
    if (m_grid[r][c] == '.')
        m_grid[r][c] = 'A';
    else if (m_grid[r][c] >= 65 && m_grid[r][c] < 90)
        m_grid[r][c]++;
    else if (m_grid[r][c] == 90)
        m_grid[r][c] = 'Z';
    return true;
}

void History::display() const {
    clearScreen();
    for (int i = 1; i <= m_rows; i++) {
        for (int j = 1; j <= m_cols; j++) {
            cout << m_grid[i][j];
        }
        cout << '\n';
    }
    cout<< '\n';
}
