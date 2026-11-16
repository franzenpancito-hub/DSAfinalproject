// ============================================================================
//  StoreApp.h - Application layer: menus + glue between all modules
// ============================================================================
#ifndef STOREAPP_H
#define STOREAPP_H

#include "Catalog.h"
#include "ProductBST.h"
#include "ShoppingCart.h"
#include "BrandRing.h"
#include "OrderQueue.h"
#include "OrderStack.h"

class StoreApp {
private:
    Catalog catalog;
    ProductBST index;
    ShoppingCart cart;
    BrandRing brands;
    OrderQueue checkoutLine;
    OrderStack undoStack;
    OrderStack redoStack;
    int nextOrderNo;

    void seed();
    Product* makeCopy() const;          // caller must delete[]

    void viewCatalog();                 // 1
    void filterByBrand();               // 2
    void keywordSearch();               // 3  (linear search)
    void priceRange();                  // 4  (binary search)

    void showCart();                    // 5  (cart)
    void addToCart();
    void placeOrder();
    void cartMenu();

    bool stockCovers(const Order& o);   // 6  (checkout queue)
    void applyStock(const Order& o, int sign);
    void processCheckout();

    void undoMenu();                    // 7  (stack)
    void brandShowcase();               // 8  (circular list)
    void sortInventory();               // 9  (sorting)
    void printMenu();

public:
    StoreApp();
    void run();
};

#endif
