//
//  Set.cpp
//  Project2
//
//  Created by Sadhana Vadrevu on 1/24/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include "Set.h"

Set::Set() {
    head = nullptr;
    tail = nullptr;
    m_size = 0;
}

Set::Set(const Set& src) {
    m_size = src.m_size;
    if (src.head == nullptr) {
        head = nullptr;
        tail = nullptr;
    }
    else {
        head = new Node;
        head->previous = nullptr;
        head->next = nullptr;
        head->value = src.head->value;
        Node* currentNode = head;
        Node* p = src.head->next;
        while (p != nullptr) {
            currentNode->next = new Node;
            Node* t_previous = currentNode;
            currentNode = currentNode->next;
            currentNode->value = p->value;
            currentNode->previous = t_previous;
            p = p->next;
        }
        currentNode->next = nullptr;
        tail = currentNode;
    }
}

Set Set::operator=(const Set& src) {
    Node* start = head;
    while (start != nullptr) {
        Node* n = start->next;
        delete start;
        start = n;
    }
    m_size = src.m_size;
    if (src.head == nullptr) {
        head = nullptr;
        tail = nullptr;
    }
    else {
        head = new Node;
        head->previous = nullptr;
        head->next = nullptr;
        head->value = src.head->value;
        Node* newNode = head;
        Node* p = src.head->next;
        while (p != nullptr) {
            newNode->next = new Node;
            Node* t_previous = newNode;
            newNode = newNode->next;
            newNode->value = p->value;
            newNode->previous = t_previous;
            p = p->next;
        }
        newNode->next = nullptr;
        tail = newNode;
    }
    return *this;
}

Set::~Set() {
    Node* p = head;
    while (p != nullptr) {
        Node* n = p->next;
        delete p;
        p = n;
    }
}

bool Set::empty() const {
    return (head == nullptr);
}

int Set::size() const {
    return m_size;
}

bool Set::insert(const ItemType& value) {
    if (contains(value) == true)
        return false;
    Node* newNode = new Node;
    newNode->value = value;
    if (empty()) {
        head = newNode;
        tail = newNode;
        newNode->previous = nullptr;
        newNode->next = nullptr;
    }
    else {
        tail->next = newNode;
        newNode->next = nullptr;
        newNode->previous = tail;
        tail = newNode;
    }
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value) {
    if (empty())
        return false;
    else if (head->value == value) {
        Node* killMe = head;
        head = killMe->next;
        head->previous = nullptr;
        delete killMe;
        m_size--;
        return true;
    }
    else if (tail->value == value) {
        Node* killMe = tail;
        tail = killMe->previous;
        tail->next = nullptr;
        delete killMe;
        m_size--;
        return true;
    }
    else {
        Node* p = head;
        while (p != nullptr) {
            if (p->next != nullptr && p->next->value == value)
                break;
        p = p->next;
        }
        if (p != nullptr) {
            Node* killMe = p->next;
            p->next = killMe->next;
            killMe->next->previous = p;
            delete killMe;
            m_size--;
            return true;
        }
        return false;
    }
}

bool Set::contains(const ItemType& value) const {
    if (empty())
        return false;
    Node* p = head;
    while (p != nullptr) {
        if (p->value == value)
            return true;
        p = p->next;
    }
    return false;
}

bool Set::get(int pos, ItemType& value) const {
    if (pos < 0 || pos >= m_size)
        return false;
    int greaterThan;
    Node* p = head;
    while (p != nullptr) {
        Node* compareTo = head;
        greaterThan = 0;
        while (compareTo != nullptr && greaterThan <= pos) {
            if (p->value > compareTo->value)
                greaterThan++;
            compareTo = compareTo->next;
        }
        if (greaterThan == pos)
            break;
        p = p->next;
    }
    value = p->value;
    return true;
}

void Set::swap(Set& other) {
    Node* t_head = head;
    head = other.head;
    other.head = t_head;
    Node* t_tail = tail;
    tail = other.tail;
    other.tail = t_tail;
    int t_size = m_size;
    m_size = other.m_size;
    other.m_size = t_size;
}

void unite (const Set& s1, const Set& s2, Set& result) {
    Set resultSet;
    for (int i = 0; i < s1.size(); i++) {
        ItemType m;
        s1.get(i, m);
        resultSet.insert(m);
    }
    for (int i = 0; i < s2.size(); i++) {
        ItemType m;
        s2.get(i, m);
        resultSet.insert(m);
    }
    result = resultSet;
}

void subtract(const Set& s1, const Set& s2, Set& result) {
    Set resultSet;
    for (int i = 0; i < s1.size(); i++) {
        ItemType m;
        s1.get(i, m);
        bool found = false;
        for (int j = 0; j < s2.size(); j++) {
            ItemType k;
            s2.get(j, k);
            if (m == k)
                found = true;
        }
        if (!found)
            resultSet.insert(m);
    }
    result = resultSet;
}
