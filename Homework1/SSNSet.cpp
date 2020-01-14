//
//  SSNSet.cpp
//  Homework1
//
//  Created by Sadhana Vadrevu on 1/20/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include "SSNSet.h"
#include "Set.h"
#include <iostream>
using namespace std;

SSNSet::SSNSet() {

}

bool SSNSet::add(unsigned long ssn) {
   return m_set.insert(ssn);
}

int SSNSet::size() const {
    return m_set.size();
}

void SSNSet::print() const {
    unsigned long toPrint;
    for (int i = 0; i < m_set.size(); i++) {
        m_set.get(i, toPrint);
        cout << toPrint << endl;
    }
}
