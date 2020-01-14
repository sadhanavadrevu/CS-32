//
//  newSet.cpp
//  Homework1
//
//  Created by Sadhana Vadrevu on 1/20/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include "newSet.h"

Set::Set(int i) {
    m_size = 0;
    if (i > 0)
        m_maxsize = i;
    else
        m_maxsize = DEFAULT_MAX_ITEMS;
    m_array = new ItemType[m_maxsize];
}

Set::Set (const Set& src) {
    m_maxsize = src.m_maxsize;
    m_array = new ItemType[m_maxsize];
    m_size = src.m_size;
    for (int i = 0; i < src.m_size; i++) {
        m_array[i] = src.m_array[i];
    }
}

Set Set::operator= (const Set& src) {
    delete [] m_array;
    m_maxsize = src.m_maxsize;
    m_size = src.m_size;
    m_array = new ItemType[m_maxsize];
    for (int i = 0; i < m_size; i++) {
        m_array[i] = src.m_array[i];
    }
    return *this;
}
Set::~Set() {
    delete [] m_array;
}

bool Set::empty() const {
    return m_size == 0;
}

int Set::size() const {
    return m_size;
}

bool Set::insert(const ItemType& value) {
    if (contains(value) == true || m_size >= m_maxsize)
        return false;
    else {
        m_array[m_size] = value;
        m_size++;
        return true;
    }
}

bool Set::erase(const ItemType &value) {
    int match = -1;
    for (int i = 0; i < m_size; i++) {
        if (value == m_array[i]) {
            match = i;
            break;
        }
    }
    if (match == -1)
        return false;
    else {
        for (int j = match; j < m_size - 1; j++) {
            m_array[j] = m_array[j+1];
        }
        m_size--;
        return true;
    }
}

bool Set::contains(const ItemType &value) const {
    for (int i = 0; i < m_size; i++) {
        if (value == m_array[i])
            return true;
    }
    return false;
}

bool Set::get(int i, ItemType& value) const {
    if (i < 0 || i >= m_size)
        return false;
    int k, greaterThan;
    for (k = 0; k < m_size; k++) {
        greaterThan = 0;
        for (int j = 0; j < m_size ; j++) {
            if (m_array[k] > m_array[j])
                greaterThan++;
            if (greaterThan > i)
                break;
        }
        if (greaterThan == i)
            break;
    }
    value = m_array[k];
    return true;
}

void Set::swap(Set& other){
    int temp_maxsize = m_maxsize;
    m_maxsize = other.m_maxsize;
    other.m_maxsize = temp_maxsize;
    ItemType* temparray = new ItemType[temp_maxsize];
    int tempsize = m_size;
    m_size = other.m_size;
    other.m_size = tempsize;
    for (int i = 0; i < other.m_maxsize; i++) { //creating a temporary array with this set's items in it
        temparray[i] = m_array[i];
    }
    delete [] m_array;
    m_array = new ItemType[m_maxsize];
    for (int i = 0; i < m_maxsize; i++) { //replacing items in this array with items from the "other" array
        m_array[i] = other.m_array[i];
    }
    delete [] other.m_array;
    other.m_array = new ItemType[other.m_maxsize];
    for (int i = 0; i < other.m_maxsize; i++) {
        other.m_array[i] = temparray[i];
    }
    delete [] temparray;
}
 q
