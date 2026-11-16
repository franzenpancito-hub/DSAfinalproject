// ============================================================================
//  Sorting.cpp - LAB 8
// ============================================================================
#include "Sorting.h"
#include "Utils.h"

bool before(const Product& a, const Product& b, int field, bool asc) {
    int cmp = 0;
    if (field == BY_PRICE) cmp = (a.price < b.price) ? -1 : (a.price > b.price ? 1 : 0);
    else if (field == BY_STOCK) cmp = (a.stockQuantity < b.stockQuantity) ? -1
                                    : (a.stockQuantity > b.stockQuantity ? 1 : 0);
    else cmp = toLower(a.brand).compare(toLower(b.brand));
    if (cmp == 0) return a.id < b.id;       // stable tie-break by ID
    return asc ? (cmp < 0) : (cmp > 0);
}

void insertionSort(Product* arr, int n, int field, bool asc) {
    for (int i = 1; i < n; i++) {
        Product key = arr[i];
        int j = i - 1;
        while (j >= 0 && before(key, arr[j], field, asc)) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void quickSort(Product* arr, int lo, int hi, int field, bool asc) {
    if (lo >= hi) return;
    Product pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (before(arr[j], pivot, field, asc)) {
            i++;
            Product tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
        }
    }
    Product tmp = arr[i + 1]; arr[i + 1] = arr[hi]; arr[hi] = tmp;
    quickSort(arr, lo, i, field, asc);
    quickSort(arr, i + 2, hi, field, asc);
}
