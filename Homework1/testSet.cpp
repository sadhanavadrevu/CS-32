//
//  main.cpp
//  Homework1
//
//  Created by Sadhana Vadrevu on 1/20/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include "Set.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()
{
    Set ss;
    assert(ss.insert("roti"));
    assert(ss.insert("pita"));
    assert(ss.size() == 2);
    assert(ss.contains("pita"));
    ItemType x = "laobing";
    assert(ss.get(0, x)  &&  x == "pita");
    assert(ss.get(1, x)  &&  x == "roti");
    assert(ss.erase("roti"));
    assert(ss.size() == 1);
    Set m;
    assert(m.insert("ice cream"));
    assert(m.insert("chocolate"));
    assert(m.insert("pie"));
    assert(m.size() == 3);
    ItemType t = "dessert";
    assert(!m.get(5, t) && t == "dessert");
    assert(m.get(2, t) && t == "pie");
    m.swap(ss);
    assert(ss.size() == 3 && m.size() == 1);
    assert(ss.get(2,x) && x == "pie");
    Set newM = m;
    assert(newM.size() == 1);
    assert(newM.contains("pita"));
    cout << "Passed all tests" << endl;
}

