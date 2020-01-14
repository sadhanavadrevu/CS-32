//
//  testnewSet.cpp
//  Homework1
//
//  Created by Sadhana Vadrevu on 1/20/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include "newSet.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int main() {
    Set a(1000);   // a can hold at most 1000 distinct items
    Set b(5);      // b can hold at most 5 distinct items
    Set c;         // c can hold at most DEFAULT_MAX_ITEMS distinct items
    Set d(-2);
    
    ItemType v[6] = { "hello", "hi", "purple", "tomato", "butterfly", "yoyo" };
    // No failures inserting 5 distinct items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v[k]));
    
    // Failure if we try to insert a sixth distinct item into b
    assert(!b.insert(v[5]));
    
    // When two Sets' contents are swapped, their capacities are swapped
    // as well:
    a.swap(b);
    assert(!a.insert(v[5]) && b.insert(v[5]));
    assert(a.size() == 5 && b.size() == 1);
    assert(a.erase("hi") && a.size() == 4);
    ItemType k = "jam";
    assert(!a.get(8, k) && k == "jam");
    assert(a.get(2, k) && k == "purple");
    cout << "All tests passed!" << endl;
}

