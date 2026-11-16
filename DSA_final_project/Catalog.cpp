// ============================================================================
//  Catalog.cpp - LAB 1
// ============================================================================
#include "Catalog.h"

Catalog::Catalog(int startCapacity) : count(0), capacity(startCapacity) {
    items = new Product[capacity];
}

Catalog::~Catalog() { delete[] items; }

void Catalog::resize(int newCapacity) {
    Product* newArr = new Product[newCapacity];
    for (int i = 0; i < count; i++) newArr[i] = items[i];
    delete[] items;
    items = newArr;
    capacity = newCapacity;
}

void Catalog::add(const Product& p) {
    if (count == capacity) resize(capacity * 2);   // grow when full
    items[count++] = p;
}

void Catalog::shrinkToFit() {                      // release unused slots
    if (capacity > count) resize(count);
}
