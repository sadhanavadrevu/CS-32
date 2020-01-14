//
//  testSSNSet.cpp
//  Homework1
//
//  Created by Sadhana Vadrevu on 1/20/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include <stdio.h>
#include "SSNSet.h"
#include "Set.h"
#include <cassert>
#include <iostream>
using namespace std;

int main() {
    SSNSet s;
    unsigned long long1 = 128448450;
    unsigned long long2 = 789040578;
    unsigned long long3 = 125905843;
    s.add(long1);
    s.add(long2);
    s.add(long3);
    assert(s.size() == 3);
    cout << "s " << endl;
    s.print();
    SSNSet g = s; //copy constructor
    cout << "g " << endl;
    g.print();
    SSNSet f; //assignment operator
    f = s;
    cout << "f " << endl;
    f.print();
}
