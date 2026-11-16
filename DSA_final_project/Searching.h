// ============================================================================
//  Searching.h - LAB 7: Binary search helpers (array sorted by price, asc)
// ============================================================================
#ifndef SEARCHING_H
#define SEARCHING_H

#include "Models.h"

// first index whose price >= value
int lowerBound(const Product* a, int n, double value, int& steps);

// first index whose price > value
int upperBound(const Product* a, int n, double value, int& steps);

#endif
