// ============================================================================
//  StoreApp.cpp - Application layer
// ============================================================================
#include "StoreApp.h"
#include "Utils.h"
#include "Sorting.h"
#include "Searching.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

StoreApp::StoreApp() : nextOrderNo(1) { seed(); }

void StoreApp::seed() {
    // Catalog starts at capacity 4 and grows by doubling (Lab 1).
    catalog.add({101, "iPhone 15 Pro 256GB",    "Smartphone", "Apple",    69990.00, 12});
    catalog.add({102, "MacBook Air M3 13-inch", "Laptop",     "Apple",    74990.00,  8});
    catalog.add({103, "AirPods Pro 2",          "Audio",      "Apple",    14990.00, 25});
    catalog.add({104, "iPad 10th Gen 64GB",     "Tablet",     "Apple",    24990.00, 10});
    catalog.add({105, "Galaxy S24 Ultra",       "Smartphone", "Samsung",  79990.00,  9});
    catalog.add({106, "Galaxy Tab S9 FE",       "Tablet",     "Samsung",  25990.00, 14});
    catalog.add({107, "Galaxy Buds2 Pro",       "Audio",      "Samsung",   9990.00, 30});
    catalog.add({108, "Galaxy Watch6 44mm",     "Wearable",   "Samsung",  18990.00,  5});
    catalog.add({109, "Sony WH-1000XM5",        "Audio",      "Sony",     21990.00, 15});
    catalog.add({110, "Sony Xperia 1 V",        "Smartphone", "Sony",     64990.00,  4});
    catalog.add({111, "Sony ZV-E10 Camera",     "Camera",     "Sony",     42990.00,  7});
    catalog.add({112, "PlayStation 5 Slim",     "Gaming",     "Sony",     32990.00,  0});
    catalog.add({113, "Logitech MX Master 3S",  "Accessory",  "Logitech",  5495.00, 40});
    catalog.add({114, "Logitech G Pro X Keyboard", "Accessory", "Logitech", 7995.00, 22});
    catalog.add({115, "Logitech C920 Webcam",   "Accessory",  "Logitech",  4295.00, 35});
    catalog.add({116, "Logitech G502 X Mouse",  "Accessory",  "Logitech",  4995.00, 28});
    catalog.shrinkToFit();

    index.buildFrom(catalog);
    for (int i = 0; i < catalog.size(); i++) brands.addBrand(catalog.at(i).brand);
}

Product* StoreApp::makeCopy() const {
    int n = catalog.size();
    Product* copy = new Product[n];
    for (int i = 0; i < n; i++) copy[i] = catalog.at(i);
    return copy;
}

// ---------------- 1. View complete catalog ----------------
void StoreApp::viewCatalog() {
    cout << "\n  COMPLETE CATALOG  (" << catalog.size() << " products, array capacity "
         << catalog.cap() << ")\n";
    printProductHeader();
    for (int i = 0; i < catalog.size(); i++) printProductRow(catalog.at(i));
    printLine();
}

// ---------------- 2. Filter by brand ----------------
void StoreApp::filterByBrand() {
    cout << "\n  PARTNER BRANDS\n";
    BrandNode* n = brands.first();
    for (int i = 1; i <= brands.size(); i++, n = n->next)
        cout << "   " << i << ". " << n->name << "\n";
    int pick = readInt("  Choose brand number: ", 1, brands.size());

    n = brands.first();
    for (int i = 1; i < pick; i++) n = n->next;

    int found = 0;
    printProductHeader();
    for (int i = 0; i < catalog.size(); i++) {
        if (toLower(catalog.at(i).brand) == toLower(n->name)) {
            printProductRow(catalog.at(i));
            found++;
        }
    }
    printLine();
    cout << "  " << found << " product(s) from " << n->name << ".\n";
}

// ---------------- 3. Keyword search (Linear) ----------------
void StoreApp::keywordSearch() {
    string raw = readLine("\n  Enter keyword (name, brand or category): ");
    string key = toLower(raw);
    size_t a = key.find_first_not_of(" \t");
    size_t b = key.find_last_not_of(" \t");
    if (a == string::npos) { cout << "  Keyword cannot be empty.\n"; return; }
    key = key.substr(a, b - a + 1);

    int found = 0, comparisons = 0;
    bool headerShown = false;
    for (int i = 0; i < catalog.size(); i++) {      // linear scan, O(n)
        const Product& p = catalog.at(i);
        comparisons++;
        if (toLower(p.name).find(key) != string::npos ||
            toLower(p.brand).find(key) != string::npos ||
            toLower(p.category).find(key) != string::npos) {
            if (!headerShown) { printProductHeader(); headerShown = true; }
            printProductRow(p);
            found++;
        }
    }
    if (headerShown) printLine();
    cout << "  " << found << " match(es) for \"" << key << "\" after "
         << comparisons << " comparisons (linear search).\n";
}

// ---------------- 4. Price range (Binary search) ----------------
void StoreApp::priceRange() {
    double lo = readDouble("\n  Minimum price (PHP): ", 0, 1e9);
    double hi = readDouble("  Maximum price (PHP): ", 0, 1e9);
    if (lo > hi) { cout << "  Minimum is greater than maximum. Try again.\n"; return; }

    int n = catalog.size();
    Product* sorted = makeCopy();
    quickSort(sorted, 0, n - 1, BY_PRICE, true);    // binary search needs sorted data

    int steps = 0;
    int start = lowerBound(sorted, n, lo, steps);
    int end = upperBound(sorted, n, hi, steps);

    if (start >= end) {
        cout << "  No products between " << lo << " and " << hi << ".\n";
    } else {
        printProductHeader();
        for (int i = start; i < end; i++) printProductRow(sorted[i]);
        printLine();
        cout << "  " << (end - start) << " product(s) found. Binary search steps: "
             << steps << " (linear scan would need " << n << ").\n";
    }
    delete[] sorted;
}

// ---------------- 5. Shopping cart ----------------
void StoreApp::showCart() {
    if (cart.empty()) { cout << "  Your cart is empty.\n"; return; }
    printLine();
    cout << left << "    " << setw(6) << "ID" << setw(30) << "Product" << right
         << setw(6) << "Qty" << setw(14) << "Subtotal" << "\n";
    printLine();
    double total = 0;
    for (CartNode* n = cart.first(); n != nullptr; n = n->next) {
        int steps;
        int idx = index.search(n->productId, steps);
        const Product& p = catalog.at(idx);
        double sub = p.price * n->qty;
        total += sub;
        cout << left << (n == cart.selected() ? "  > " : "    ") << setw(6) << p.id
             << setw(30) << p.name << right << setw(6) << n->qty << setw(14) << sub << "\n";
    }
    printLine();
    cout << right << setw(40) << "TOTAL (PHP): " << setw(14) << total << "\n";
    cout << "  ( > marks the item you are viewing )\n";
}

void StoreApp::addToCart() {
    int id = readInt("  Enter Product ID: ", 1, 999999);
    int steps = 0;
    int idx = index.search(id, steps);              // BST lookup (Lab 6)
    if (idx == -1) {
        cout << "  Product ID " << id << " not found (BST visited " << steps << " node(s)).\n";
        return;
    }
    const Product& p = catalog.at(idx);
    cout << "  Found via BST in " << steps << " step(s): " << p.name << " [" << p.brand
         << "] - PHP " << p.price << ", stock " << p.stockQuantity << "\n";
    int available = p.stockQuantity - cart.qtyInCart(id);
    if (available <= 0) {
        cout << "  Cannot add: no more stock available for this item.\n";
        return;
    }
    int qty = readInt("  Quantity (1-" + to_string(available) + "): ", 1, available);
    cart.addItem(id, qty);
    cout << "  Added to cart.\n";
}

void StoreApp::placeOrder() {
    if (cart.empty()) { cout << "  Cart is empty - nothing to place.\n"; return; }
    Order o;
    o.orderNo = nextOrderNo++;
    for (CartNode* n = cart.first(); n != nullptr; n = n->next)
        o.items.push_back({n->productId, n->qty});
    checkoutLine.enqueue(o);
    cart.clear();
    cout << "  Order #" << o.orderNo << " placed. Position in checkout line: "
         << checkoutLine.size() << ".\n";
}

void StoreApp::cartMenu() {
    while (true) {
        cout << "\n  --- SHOPPING CART (Doubly Linked List) ---\n"
             << "   1. Add item by Product ID\n"
             << "   2. View cart\n"
             << "   3. Next item  ->\n"
             << "   4. Previous item  <-\n"
             << "   5. Remove highlighted item\n"
             << "   6. Place order (send to checkout queue)\n"
             << "   0. Back to main menu\n";
        int c = readInt("  Choice: ", 0, 6);
        if (c == 0) return;
        if (c == 1) addToCart();
        else if (c == 2) showCart();
        else if (c == 3) {
            if (cart.empty()) cout << "  Cart is empty.\n";
            else { cart.moveNext(); showCart(); }
        } else if (c == 4) {
            if (cart.empty()) cout << "  Cart is empty.\n";
            else { cart.movePrev(); showCart(); }
        } else if (c == 5) {
            if (cart.empty()) cout << "  Cart is empty.\n";
            else { cart.removeCurrent(); cout << "  Item removed.\n"; }
        } else if (c == 6) placeOrder();
    }
}

// ---------------- 6. Process checkout (Queue) ----------------
bool StoreApp::stockCovers(const Order& o) {
    for (size_t i = 0; i < o.items.size(); i++) {
        int steps;
        int idx = index.search(o.items[i].productId, steps);
        if (idx == -1 || catalog.at(idx).stockQuantity < o.items[i].qty) return false;
    }
    return true;
}

void StoreApp::applyStock(const Order& o, int sign) {     // -1 deduct, +1 restore
    for (size_t i = 0; i < o.items.size(); i++) {
        int steps;
        int idx = index.search(o.items[i].productId, steps);
        catalog.at(idx).stockQuantity += sign * o.items[i].qty;
    }
}

void StoreApp::processCheckout() {
    if (checkoutLine.empty()) { cout << "\n  Checkout line is empty.\n"; return; }
    Order o;
    checkoutLine.dequeue(o);                    // FIFO: oldest order first
    cout << "\n  Processing Order #" << o.orderNo << " (first in line)...\n";

    if (!stockCovers(o)) {
        cout << "  REJECTED: not enough stock for one or more items. Order cancelled.\n";
    } else {
        applyStock(o, -1);
        double total = 0;
        printLine('-', 60);
        for (size_t i = 0; i < o.items.size(); i++) {
            int steps;
            const Product& p = catalog.at(index.search(o.items[i].productId, steps));
            double sub = p.price * o.items[i].qty;
            total += sub;
            cout << left << "  " << setw(28) << p.name << right << " x" << setw(3)
                 << o.items[i].qty << setw(14) << sub << "\n";
        }
        printLine('-', 60);
        cout << left << "  " << setw(33) << "TOTAL (PHP)" << right << setw(14) << total << "\n";
        cout << "  Order #" << o.orderNo << " completed. Stock updated.\n";
        undoStack.push(o);                      // remember it for Undo (Lab 4)
        redoStack.clear();                      // new action invalidates redo history
    }
    cout << "  Orders still waiting: " << checkoutLine.size() << "\n";
}

// ---------------- 7. Admin undo / redo (Stack) ----------------
void StoreApp::undoMenu() {
    cout << "\n  --- ADMIN UNDO / REDO (Stack) ---\n"
         << "  Undo history: " << undoStack.size() << "   Redo history: "
         << redoStack.size() << "\n"
         << "   1. Undo last processed order (restore stock)\n"
         << "   2. Redo last undone order\n"
         << "   0. Back\n";
    int c = readInt("  Choice: ", 0, 2);
    Order o;
    if (c == 1) {
        if (!undoStack.pop(o)) { cout << "  Nothing to undo.\n"; return; }
        applyStock(o, +1);
        redoStack.push(o);
        cout << "  Order #" << o.orderNo << " reversed. Stock restored.\n";
    } else if (c == 2) {
        if (!redoStack.pop(o)) { cout << "  Nothing to redo.\n"; return; }
        if (!stockCovers(o)) {
            cout << "  Cannot redo Order #" << o.orderNo << ": stock is no longer enough.\n";
            redoStack.push(o);
            return;
        }
        applyStock(o, -1);
        undoStack.push(o);
        cout << "  Order #" << o.orderNo << " re-applied. Stock deducted again.\n";
    }
}

// ---------------- 8. Brand banner showcase (Circular list) ----------------
void StoreApp::brandShowcase() {
    if (brands.first() == nullptr) { cout << "  No brands.\n"; return; }
    BrandNode* cur = brands.first();
    int position = 1;
    cout << "\n  Press Enter for the next banner (it loops forever), or type Q to quit.\n";
    while (true) {
        int items = 0, stock = 0;
        double lo = 0, hi = 0;
        for (int i = 0; i < catalog.size(); i++) {
            const Product& p = catalog.at(i);
            if (toLower(p.brand) != toLower(cur->name)) continue;
            if (items == 0) { lo = hi = p.price; }
            if (p.price < lo) lo = p.price;
            if (p.price > hi) hi = p.price;
            items++;
            stock += p.stockQuantity;
        }
        cout << "\n  +------------------------------------------+\n"
             << "  |  FEATURED PARTNER " << position << " of " << brands.size() << "\n"
             << "  |  " << cur->name << "\n"
             << "  |  Products: " << items << "   Units in stock: " << stock << "\n"
             << "  |  Price range: PHP " << lo << " - " << hi << "\n"
             << "  +------------------------------------------+\n";

        string s = readLine("  [Enter] next / [Q] quit: ");
        if (!s.empty() && (s[0] == 'q' || s[0] == 'Q')) break;

        cur = cur->next;                         // circular: last -> first
        position = (position % brands.size()) + 1;
        if (position == 1) cout << "  (Looped back to the first brand)\n";
    }
}

// ---------------- 9. Sort inventory ----------------
void StoreApp::sortInventory() {
    cout << "\n  SORT BY\n"
         << "   1. Price  (Quick Sort)\n"
         << "   2. Stock  (Insertion Sort)\n"
         << "   3. Brand  (Insertion Sort)\n";
    int field = readInt("  Choice: ", 1, 3);
    cout << "   1. Ascending\n   2. Descending\n";
    bool asc = readInt("  Order: ", 1, 2) == 1;

    int n = catalog.size();
    Product* copy = makeCopy();                  // original catalog order stays intact
    if (field == BY_PRICE) quickSort(copy, 0, n - 1, field, asc);
    else insertionSort(copy, n, field, asc);

    printProductHeader();
    for (int i = 0; i < n; i++) printProductRow(copy[i]);
    printLine();
    delete[] copy;
}

void StoreApp::printMenu() {
    cout << "\n================================================================\n"
         << "  GADGET STORE INVENTORY & SEARCH SYSTEM (CS201)\n"
         << "================================================================\n"
         << "1. View Complete Catalog (Tabular View with Brand Affiliations)\n"
         << "2. Filter Products by Official Brand\n"
         << "3. Search Products by Keyword (Linear Search)\n"
         << "4. Filter Products by Price Range (Binary Search)\n"
         << "5. Add Item to Shopping Cart (Doubly Linked List Navigation)\n"
         << "6. Process Customer Checkout Queue (FIFO Order Queue)\n"
         << "7. Perform Admin Undo Action (LIFO Stack Pop)\n"
         << "8. Display Partner Brand Banner Showcase (Circular List Cycle)\n"
         << "9. Sort Inventory Display (By Price, Stock, or Brand)\n"
         << "10. Exit\n"
         << "================================================================\n"
         << "  Cart items: " << cart.size() << "  |  Orders waiting: "
         << checkoutLine.size() << "\n";
}

void StoreApp::run() {
    while (true) {
        printMenu();
        int choice = readInt("Enter choice (1-10): ", 1, 10);
        switch (choice) {
            case 1: viewCatalog(); break;
            case 2: filterByBrand(); break;
            case 3: keywordSearch(); break;
            case 4: priceRange(); break;
            case 5: cartMenu(); break;
            case 6: processCheckout(); break;
            case 7: undoMenu(); break;
            case 8: brandShowcase(); break;
            case 9: sortInventory(); break;
            case 10:
                cout << "\nThank you! Freeing all memory and exiting...\n";
                return;
        }
        readLine("\n  Press Enter to continue...");
    }
}
