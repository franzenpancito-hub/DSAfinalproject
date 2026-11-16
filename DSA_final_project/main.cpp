// ============================================================================
//  GADGET STORE INVENTORY & SEARCH SYSTEM (CS201)
//  Data Structures and Algorithm Analysis - Final Output
//  Domain : Tech & Electronics
//  Brands : Apple, Samsung, Sony, Logitech
//
//  Module map (one .h/.cpp pair per lab - errors point straight to the file)
//    Lab 1  Dynamic allocation ........ Catalog.h/.cpp
//    Lab 2  Doubly linked list ........ ShoppingCart.h/.cpp
//    Lab 3  Circular linked list ...... BrandRing.h/.cpp
//    Lab 4  Stack (LIFO) .............. OrderStack.h/.cpp
//    Lab 5  Queue (FIFO) .............. OrderQueue.h/.cpp
//    Lab 6  Binary Search Tree ........ ProductBST.h/.cpp
//    Lab 7  Searching ................. Searching.h/.cpp (+ linear in StoreApp)
//    Lab 8  Sorting ................... Sorting.h/.cpp
//    Shared ........................... Models.h, Utils.h/.cpp
//    Menus / glue ..................... StoreApp.h/.cpp
// ============================================================================
#include <iostream>
#include <iomanip>
#include "StoreApp.h"
#include "Utils.h"
using namespace std;

int main() {
    cout << fixed << setprecision(2);
    try {
        StoreApp app;       // all dynamic memory is released by destructors
        app.run();
    } catch (const InputClosed&) {
        cout << "\nInput closed. Exiting safely.\n";
    }
    return 0;
}
