//
//  linear.cpp
//  hw3p2
//
//  Created by Sadhana Vadrevu on 2/11/19.
//  Copyright © 2019 Sadhana Vadrevu. All rights reserved.
//


// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
    if (n <= 0)
        return false;
    if (somePredicate(a[0]) == true) //if the predicate is true for the first element, done checking - function is true
        return true;
    
    return (anyTrue(a + 1, n - 1)); //otherwise, check if the predicate is true in the rest of the array recursively
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n)
{
    if (n <= 0)
        return 0;
    if (somePredicate(a[0]) == true) //if the predicate is true for the first element in the array
        return 1 + countTrue(a + 1, n - 1); //add 1 to the number of true elements in the rest of the array and return
    
    return countTrue(a + 1, n - 1); //otherwise, return just the number of true elements in the rest of the array
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (somePredicate(a[0])) //if the predicate is true for the first element of the array return index 0 (first position)
        return 0;
    
    int first = firstTrue(a + 1, n - 1); //otherwise, find the index of the first true element in the rest of the array
    if (first == -1) //if no true element was found, return -1
        return -1;
    else //if there was a true element
        return 1 + first; //add 1 to account for position in original array (first is position relative to smaller array)
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfMin(const double a[], int n)
{
    if (n <= 0) //function is told to examine no elements
        return -1;
    if (n == 1) //if there is only one element in the array, it is automatically the smallest element
        return 0;
    
    int smallest = 1 + positionOfMin(a + 1, n - 1); //otherwise, find the index of the smallest element in the rest of the array (recursively)
    if (a[0] < a[smallest]) //if element in position 0 is smaller than smallest (the smallest element in the rest of the array), then return 0 because the first position holds the smallest element
        return 0;
    else
        return smallest; //otherwise, the smallest element in the rest of the array is the smallest element in the entire array
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
   
    if (n1 < n2)
        return false;
    if (n2 <= 0) //if a2 is empty, a1 will always contain a2
        return true;
    
    
    if (a1[0] == a2[0]) //if the first elements of both arrays match
        return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1); //recursively check if the rest of a1 contains the rest of a2
    else
        return includes(a1 + 1, n1 - 1, a2, n2); //otherwise, recursively check the rest of a1 for all of a2
}
