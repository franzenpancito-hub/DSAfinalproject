// ============================================================================
//  Catalog.h - LAB 1: Dynamic allocation (resizable array)
// ============================================================================
#ifndef CATALOG_H
#define CATALOG_H

#include "Models.h"

class Catalog {
private:
    Product* items;
    int count;
    int capacity;

    void resize(int newCapacity);   // allocate new, copy, delete[] old

public:
    Catalog(int startCapacity = 4);
    ~Catalog();
    Catalog(const Catalog&) = delete;
    Catalog& operator=(const Catalog&) = delete;

    void add(const Product& p);
    void shrinkToFit();
    int size() const { return count; }
    int cap() const { return capacity; }
    Product& at(int i) { return items[i]; }
    const Product& at(int i) const { return items[i]; }
};

#endif
