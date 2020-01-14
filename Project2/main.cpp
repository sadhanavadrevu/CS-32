//
//  main.cpp
//  Project2
//
//  Created by Sadhana Vadrevu on 1/24/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include <iostream>
#include "Set.h"
#include <cassert>
using namespace std;

int main() {
    Set ss; //default constructor
        //for an empty set
    assert(ss.size() == 0); //test size
    assert(ss.empty()); //test empty
    assert(!ss.erase("roti")); //nothing to remove
    assert(!ss.contains("roti")); //nothing in Set
    ItemType x = "na";
    assert(!ss.get(0, x) && x == "na"); //test get when there’s nothing to get
    assert(ss.insert("roti")); //test insert for an empty Set
    assert(ss.insert("pita")); //test insert works for a non-empty Set
    assert(!ss.insert("roti")); //check that insert won’t insert a word already in the Set
        //for a non-empty Set
    assert(ss.size() == 2); //test size
    assert(ss.contains("pita")); //test contains when Set does contain word
    assert(!ss.contains("goldfish")); //test contains when Set does not contain word
    ItemType y = "laobing";
    assert(!ss.get(4, y) && y == "laobing"); //test get when pos is out of range (y remains unchanged)
    assert(ss.get(0, y)  &&  y == "pita"); //test get
    assert(ss.get(1, y)  &&  y == "roti"); //test get
    assert(ss.erase("roti")); //test erase
    assert(ss.size() == 1); //test that erase properly decrements size
    Set m;
    assert(m.insert("ice cream"));
    assert(m.insert("chocolate"));
    assert(m.insert("pie"));
    assert(m.size() == 3);
    m.swap(ss); //swap m and ss
    assert(ss.size() == 3 && m.size() == 1); //test that swap correctly switches the sizes of the Sets
    assert(ss.get(2,x) && x == "pie"); //check that Set contains the items originally in m
    Set newM = m; //copy constructor
        //check that new object newM matches m
    assert(newM.size() == 1);
    assert(newM.contains("pita"));
    newM = ss; //assignment operator
        //check that newM matches ss
    assert(newM.size() == ss.size() && ss.size() == 3);
    ItemType t = "hello";
    assert(newM.get(1, t) && t == "ice cream");
    ItemType k = "hello";
    assert(ss.get(1, k) && k == "ice cream");
        //test unite
    Set result; //result is an empty Set
    m.insert("sourdough");
    ss.insert("pita"); //now ss and m share an item
    unite(ss, m, result);
    assert(result.size() == 5); //check that result has the correct size
    assert(result.get(0, k) && k == "chocolate"); //check that result has the correct items
    ss.insert("potato");
    unite(ss, ss, result); //check for proper functionality with aliasing in s1 and s2 and when result is a non-empty set
    assert(result.size() == 5);
    assert(result.contains("pie") && !result.contains("roti"));
    unite(m, ss, ss); //check for proper functionality with aliasing in s2 and result
    assert(ss.size() == 6);
    assert(ss.get(5, k) && k == "sourdough");
        //test subtract
    Set resultsub; //resultsub is an empty Set
    subtract(ss, m, resultsub);
    assert(resultsub.size() == 4); //check that result has the correct size
    assert(resultsub.contains("pie") && !resultsub.contains("pita")); //check that result has the correct items
    subtract(ss, ss, resultsub); // check for proper functionality with aliasing in s1 and s2 and when result is a non-empty set
    assert(resultsub.empty());
    subtract(ss, m, ss); //check for proper functionality with aliasing in s1 and result
    assert(ss.size() == 4);
    assert(ss.contains("pie") && !ss.contains("pita"));
    cout << "Passed all tests" << endl;
    
}

