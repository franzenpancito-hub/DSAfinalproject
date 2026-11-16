// ============================================================================
//  Searching.cpp - LAB 7
// ============================================================================
#include "Searching.h"

int lowerBound(const Product* a, int n, double value, int& steps) {
    int lo = 0, hi = n;
    while (lo < hi) {
        steps++;
        int mid = lo + (hi - lo) / 2;
        if (a[mid].price < value) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

int upperBound(const Product* a, int n, double value, int& steps) {
    int lo = 0, hi = n;
    while (lo < hi) {
        steps++;
        int mid = lo + (hi - lo) / 2;
        if (a[mid].price <= value) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}
