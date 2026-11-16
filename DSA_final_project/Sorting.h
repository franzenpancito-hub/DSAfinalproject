// ============================================================================
//  Sorting.h - LAB 8: Insertion Sort + Quick Sort
// ============================================================================
#ifndef SORTING_H
#define SORTING_H

#include "Models.h"

enum SortField { BY_PRICE = 1, BY_STOCK = 2, BY_BRAND = 3 };

// true if a must come strictly before b
bool before(const Product& a, const Product& b, int field, bool asc);

void insertionSort(Product* arr, int n, int field, bool asc);
void quickSort(Product* arr, int lo, int hi, int field, bool asc);

#endif
