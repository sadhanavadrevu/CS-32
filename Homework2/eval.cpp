//
//  eval.cpp
//  hw2p5
//
//  Created by Sadhana Vadrevu on 2/2/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//

#include <iostream>
#include "Set.h"
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

//helper functions
string removeSpaces(string infix);
bool isValid (string infix);
string infixToPF(string infix);
bool isLessThanTop(char op, stack<char> ops);


int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result) {
    
    infix = removeSpaces(infix);
    if (!isValid(infix))
        return 1;
    postfix = infixToPF(infix);
    
    //evaluate postfix expression
    stack<bool> operands;
    for (int i = 0; i < postfix.size(); i++) {
        if (isalpha(postfix[i])) {
            if (!trueValues.contains(postfix[i]) && !falseValues.contains(postfix[i]))
                return 2;
            if (trueValues.contains(postfix[i]) && falseValues.contains(postfix[i]))
                return 3;
            if (trueValues.contains(postfix[i]))
                operands.push(true);
            else if (falseValues.contains(postfix[i]))
                operands.push(false);
        }
        else if (postfix[i] == '!') {
            bool temp = operands.top();
            operands.pop();
            temp = !temp;
            operands.push(temp);
        }
        else { //postfix[i] is a binary operator
            bool op1 = operands.top();
            operands.pop();
            bool op2 = operands.top();
            operands.pop();
            if (postfix[i] == '|') {
                if (op1 == true || op2 == true)
                    operands.push(true);
                else
                    operands.push(false);
            }
            else if (postfix[i] == '&') {
                if (op1 == true && op2 == true)
                    operands.push(true);
                else
                    operands.push(false);
            }
        }
    }
    
    result = operands.top();
    return 0;
    
}

//helper function implementations
string removeSpaces(string infix) {
    string temp = "";
    for (int i = 0; i < infix.size(); i++) {
        if (infix[i] != ' ')
            temp += infix[i];
    }
    return temp;
}

bool isValid (string infix) {
    if (infix.size() == 0)
        return false;
    if (!isalpha(infix[0]) &&  infix[0] != '(' && infix[0] != '!')
        return false;
    if (!isalpha(infix[infix.size() -1]) && infix[infix.size()-1] != ')')
        return false;
    
    int numOperands = 0;
    int numOpenP = 0;
    int numClosedP = 0;
    
    for (int i = 0; i < infix.size(); i++) {
        if (isalpha(infix[i])) {
            if (isupper(infix[i]))
                return false;
            numOperands++;
            if (i != infix.size() - 1 && isalpha(infix[i+1]))
                return false;
            if (i != infix.size() - 1 && !(infix[i+1] == '|' || infix[i+1] == '&' ||infix[i+1] == ')'))
                return false;
        }
        
        else if (infix[i] == '(') {
            numOpenP++;
            if (i != infix.size() - 1 && !(isalpha(infix[i+1]) || infix[i+1] =='!' || infix[i] == '('))
                return false;
        }
        
        else if (infix[i] == ')') {
            numClosedP++;
            if (i != infix.size() - 1 && !(infix[i+1] =='&' || infix[i] == '|' || infix[i] == ')'))
                return false;
        }
        
        else if (infix[i] == '&' || infix[i] == '|') {
            if (!isalpha(infix[i-1]) && infix[i-1] != ')')
                return false;
            if (!isalpha(infix[i+1]) && infix[i+1] != '(' && infix[i+1] != '!')
                return false;
        }
        
        else if (infix[i] == '!') {
            if (!isalpha(infix[i+1]) && infix[i+1] != '(')
                return false;
        }
        
        else
            return false;
    }
    
    if (numOperands == 0)
        return false;
    if (numOpenP != numClosedP)
        return false;
    
    return true;
}

string infixToPF (string infix) {
    string postfix = "";
    stack<char> opStack;
    for (int i = 0; i < infix.size(); i++) {
        if (isalpha(infix[i]))
            postfix += infix[i];
        else if (infix[i] == '(')
            opStack.push(infix[i]);
        else if (infix[i] == ')') {
            while (opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop();
        }
        else if (infix[i] == '!' || infix[i] == '&' || infix[i] == '|') {
            while (!opStack.empty() && opStack.top() != '(' && isLessThanTop(infix[i], opStack)) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(infix[i]);
        }
    }
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
    
    return postfix;
}

bool isLessThanTop(char op, stack<char> ops) {
    switch (ops.top()) {
        case '!':
            return true;
        case '&':
            if (op == '|' || op == '&')
                return true;
            return false;
        case '|':
            if (op == '|')
                return true;
            return false;
    }
    return false;
}

