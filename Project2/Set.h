//
//  Set.h
//  Project2
//
//  Created by Sadhana Vadrevu on 1/24/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#ifndef Set_h
#define Set_h

#include <stdio.h>
#include <string>

using ItemType = std::string;

class Set
{
public:
    Set();
    Set(const Set& src);
    Set operator= (const Set& src);
    ~Set();
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
    
private:
    
    struct Node {
        ItemType value;
        Node* next;
        Node* previous;
    };
    
    Node* head;
    Node* tail;
    int m_size;
};

void unite (const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);

#endif /* Set_h */
