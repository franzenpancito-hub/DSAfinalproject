// ============================================================================
//  DATA STRUCTURE AND ALGORITHM ANALYSIS - FINAL OUTPUT
//  Brand-Affiliated E-Commerce Inventory and Search Engine System (C++)
//  Niche: Tech & Electronics   |   Partner Brands: Apple, Samsung, Sony, Asus
//
//  Lab mapping (see README.md for details):
//    Lab 1 - Structs, dynamic arrays, pointers (struct Product, toArray)
//    Lab 2 - Singly linked list      (Inventory list)
//    Lab 3 - Stack                   (Undo stack for admin actions)
//    Lab 4 - Queue                   (Checkout / order queue)
//    Lab 5 - Binary Search Tree      (ID index of products)
//    Lab 6 - Searching               (Linear search, Binary search, BST search)
//    Lab 7 - Elementary sorts        (Bubble, Selection, Insertion)
//    Lab 8 - Advanced sorts          (Merge sort, Quick sort)
// ============================================================================
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

// ---------------------------------------------------------------------------
// Product model
// ---------------------------------------------------------------------------
struct Product {
    int id;             // Unique Identifier
    string name;        // Product Title
    string category;    // Category (e.g., Smartphone, Laptop)
    string brand;       // Official Partner Brand
    double price;       // Unit Price (PHP)
    int stockQuantity;  // Available Inventory Level
};

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
string toLower(string s) {
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

int readInt(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) return minVal;  // EOF safety
        try {
            size_t pos = 0;
            int v = stoi(line, &pos);
            if (pos == line.size() && v >= minVal && v <= maxVal) return v;
        } catch (...) {}
        cout << "  [!] Invalid input. Enter a number from " << minVal << " to " << maxVal << ".\n";
    }
}

double readPrice(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) return 0.01;
        try {
            size_t pos = 0;
            double v = stod(line, &pos);
            if (pos == line.size() && v > 0) return v;
        } catch (...) {}
        cout << "  [!] Invalid price. Enter a positive number.\n";
    }
}

string readText(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        if (!getline(cin, line)) return "N/A";
        if (!line.empty()) return line;
        cout << "  [!] Input cannot be empty.\n";
    }
}

void printHeader() {
    cout << left << setw(5) << "ID" << setw(28) << "Name" << setw(13) << "Category"
         << setw(10) << "Brand" << right << setw(12) << "Price (PHP)" << setw(8) << "Stock" << "\n";
    cout << string(76, '-') << "\n";
}

void printProduct(const Product& p) {
    cout << left << setw(5) << p.id << setw(28) << p.name.substr(0, 27) << setw(13) << p.category.substr(0, 12)
         << setw(10) << p.brand.substr(0, 9) << right << setw(12) << fixed << setprecision(2) << p.price
         << setw(8) << p.stockQuantity << "\n";
}

// ---------------------------------------------------------------------------
// LAB 2 + LAB 5: Inventory = singly linked list + BST index (by id)
// ---------------------------------------------------------------------------
struct ListNode {
    Product data;
    ListNode* next;
    ListNode(const Product& p) : data(p), next(nullptr) {}
};

struct BSTNode {
    int id;
    ListNode* node;  // points to the product in the linked list
    BSTNode *left, *right;
    BSTNode(int i, ListNode* n) : id(i), node(n), left(nullptr), right(nullptr) {}
};

class Inventory {
    ListNode* head;
    ListNode* tail;
    BSTNode* root;
    int count;

    // ---- BST internals (Lab 5) ----
    void insertBST(BSTNode*& r, int id, ListNode* n) {
        if (!r) { r = new BSTNode(id, n); return; }
        if (id < r->id) insertBST(r->left, id, n);
        else insertBST(r->right, id, n);
    }
    BSTNode* removeBST(BSTNode* r, int id) {
        if (!r) return nullptr;
        if (id < r->id) r->left = removeBST(r->left, id);
        else if (id > r->id) r->right = removeBST(r->right, id);
        else {
            if (!r->left) { BSTNode* t = r->right; delete r; return t; }
            if (!r->right) { BSTNode* t = r->left; delete r; return t; }
            BSTNode* m = r->right;                 // in-order successor
            while (m->left) m = m->left;
            r->id = m->id;
            r->node = m->node;
            r->right = removeBST(r->right, m->id);
        }
        return r;
    }
    void freeBST(BSTNode* r) {
        if (!r) return;
        freeBST(r->left);
        freeBST(r->right);
        delete r;
    }
    void inorder(BSTNode* r, bool& first) const {
        if (!r) return;
        inorder(r->left, first);
        cout << (first ? "" : " -> ") << r->id;
        first = false;
        inorder(r->right, first);
    }
    int height(BSTNode* r) const {
        if (!r) return 0;
        int l = height(r->left), rr = height(r->right);
        return 1 + (l > rr ? l : rr);
    }

public:
    Inventory() : head(nullptr), tail(nullptr), root(nullptr), count(0) {}
    ~Inventory() {
        ListNode* cur = head;
        while (cur) { ListNode* nx = cur->next; delete cur; cur = nx; }
        freeBST(root);
    }
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    int size() const { return count; }
    ListNode* first() const { return head; }

    // Lab 5: O(log n) average search by id
    Product* findById(int id) const {
        BSTNode* cur = root;
        while (cur) {
            if (id == cur->id) return &cur->node->data;
            cur = (id < cur->id) ? cur->left : cur->right;
        }
        return nullptr;
    }

    bool addProduct(const Product& p) {
        if (findById(p.id)) return false;  // unique id
        ListNode* n = new ListNode(p);
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
        insertBST(root, p.id, n);
        count++;
        return true;
    }

    bool removeProduct(int id, Product* removed = nullptr) {
        if (!findById(id)) return false;
        ListNode *prev = nullptr, *cur = head;
        while (cur && cur->data.id != id) { prev = cur; cur = cur->next; }
        if (!cur) return false;
        if (removed) *removed = cur->data;
        if (prev) prev->next = cur->next; else head = cur->next;
        if (cur == tail) tail = prev;
        root = removeBST(root, id);
        delete cur;
        count--;
        return true;
    }

    int maxId() const {
        int m = 0;
        for (ListNode* c = head; c; c = c->next) if (c->data.id > m) m = c->data.id;
        return m;
    }

    // Lab 1: dynamically allocated array copy (caller must delete[])
    Product* toArray(int& n) const {
        n = count;
        if (count == 0) return nullptr;
        Product* arr = new Product[count];
        int i = 0;
        for (ListNode* c = head; c; c = c->next) arr[i++] = c->data;
        return arr;
    }

    void printInorderIds() const {
        if (!root) { cout << "(empty tree)\n"; return; }
        bool first = true;
        cout << "BST in-order (sorted IDs): ";
        inorder(root, first);
        cout << "\nTree height: " << height(root) << "\n";
    }
};

// ---------------------------------------------------------------------------
// LAB 3: Stack (linked) - Undo for admin actions
// ---------------------------------------------------------------------------
enum ActionType { ACT_ADD, ACT_REMOVE, ACT_RESTOCK };

struct Action {
    ActionType type;
    Product product;  // snapshot (for ADD: the added product, REMOVE: the removed one)
    int oldStock;     // for RESTOCK
};

struct ActionNode {
    Action data;
    ActionNode* next;
};

class UndoStack {
    ActionNode* topNode;
    int count;
public:
    UndoStack() : topNode(nullptr), count(0) {}
    ~UndoStack() { while (!isEmpty()) { Action a; pop(a); } }
    UndoStack(const UndoStack&) = delete;
    UndoStack& operator=(const UndoStack&) = delete;

    bool isEmpty() const { return topNode == nullptr; }
    int size() const { return count; }
    void push(const Action& a) {
        ActionNode* n = new ActionNode{a, topNode};
        topNode = n;
        count++;
    }
    bool pop(Action& out) {
        if (!topNode) return false;
        ActionNode* t = topNode;
        out = t->data;
        topNode = t->next;
        delete t;
        count--;
        return true;
    }
    const Action* peek() const { return topNode ? &topNode->data : nullptr; }
};

// ---------------------------------------------------------------------------
// LAB 4: Queue (linked) - Checkout orders (FIFO)
// ---------------------------------------------------------------------------
struct Order {
    int orderId;
    string customer;
    int productId;
    int quantity;
};

struct OrderNode {
    Order data;
    OrderNode* next;
};

class OrderQueue {
    OrderNode *frontNode, *rearNode;
    int count;
public:
    OrderQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}
    ~OrderQueue() { while (!isEmpty()) { Order o; dequeue(o); } }
    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;

    bool isEmpty() const { return frontNode == nullptr; }
    int size() const { return count; }
    void enqueue(const Order& o) {
        OrderNode* n = new OrderNode{o, nullptr};
        if (!rearNode) frontNode = rearNode = n;
        else { rearNode->next = n; rearNode = n; }
        count++;
    }
    bool dequeue(Order& out) {
        if (!frontNode) return false;
        OrderNode* t = frontNode;
        out = t->data;
        frontNode = t->next;
        if (!frontNode) rearNode = nullptr;
        delete t;
        count--;
        return true;
    }
    void display() const {
        if (!frontNode) { cout << "  No pending orders.\n"; return; }
        int pos = 1;
        for (OrderNode* c = frontNode; c; c = c->next, pos++)
            cout << "  #" << pos << "  Order " << c->data.orderId << " | " << c->data.customer
                 << " | Product ID " << c->data.productId << " | Qty " << c->data.quantity << "\n";
    }
};

// ---------------------------------------------------------------------------
// LAB 7 + LAB 8: Sorting algorithms on a dynamic array of Product
// key: 1=price, 2=name, 3=stock, 4=id
// ---------------------------------------------------------------------------
int compareBy(const Product& a, const Product& b, int key) {
    switch (key) {
        case 1: return (a.price < b.price) ? -1 : (a.price > b.price) ? 1 : 0;
        case 2: { string x = toLower(a.name), y = toLower(b.name); return x < y ? -1 : (x > y ? 1 : 0); }
        case 3: return (a.stockQuantity < b.stockQuantity) ? -1 : (a.stockQuantity > b.stockQuantity) ? 1 : 0;
        default: return (a.id < b.id) ? -1 : (a.id > b.id) ? 1 : 0;
    }
}

// "a should come after b" given direction
bool outOfOrder(const Product& a, const Product& b, int key, bool asc) {
    int c = compareBy(a, b, key);
    return asc ? c > 0 : c < 0;
}

void swapP(Product& a, Product& b) { Product t = a; a = b; b = t; }

// Lab 7
void bubbleSort(Product* arr, int n, int key, bool asc) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++)
            if (outOfOrder(arr[j], arr[j + 1], key, asc)) { swapP(arr[j], arr[j + 1]); swapped = true; }
        if (!swapped) break;
    }
}

void selectionSort(Product* arr, int n, int key, bool asc) {
    for (int i = 0; i < n - 1; i++) {
        int best = i;
        for (int j = i + 1; j < n; j++)
            if (outOfOrder(arr[best], arr[j], key, asc)) best = j;
        if (best != i) swapP(arr[i], arr[best]);
    }
}

void insertionSort(Product* arr, int n, int key, bool asc) {
    for (int i = 1; i < n; i++) {
        Product cur = arr[i];
        int j = i - 1;
        while (j >= 0 && outOfOrder(arr[j], cur, key, asc)) { arr[j + 1] = arr[j]; j--; }
        arr[j + 1] = cur;
    }
}

// Lab 8
void mergeRange(Product* arr, Product* tmp, int l, int m, int r, int key, bool asc) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (!outOfOrder(arr[i], arr[j], key, asc)) tmp[k++] = arr[i++];
        else tmp[k++] = arr[j++];
    }
    while (i <= m) tmp[k++] = arr[i++];
    while (j <= r) tmp[k++] = arr[j++];
    for (int x = l; x <= r; x++) arr[x] = tmp[x];
}
void mergeSortRec(Product* arr, Product* tmp, int l, int r, int key, bool asc) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortRec(arr, tmp, l, m, key, asc);
    mergeSortRec(arr, tmp, m + 1, r, key, asc);
    mergeRange(arr, tmp, l, m, r, key, asc);
}
void mergeSort(Product* arr, int n, int key, bool asc) {
    if (n < 2) return;
    Product* tmp = new Product[n];
    mergeSortRec(arr, tmp, 0, n - 1, key, asc);
    delete[] tmp;
}

int partitionP(Product* arr, int lo, int hi, int key, bool asc) {
    int mid = lo + (hi - lo) / 2;
    swapP(arr[mid], arr[hi]);  // middle pivot avoids worst case on sorted data
    Product pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++)
        if (!outOfOrder(arr[j], pivot, key, asc)) { i++; swapP(arr[i], arr[j]); }
    swapP(arr[i + 1], arr[hi]);
    return i + 1;
}
void quickSortRec(Product* arr, int lo, int hi, int key, bool asc) {
    if (lo >= hi) return;
    int p = partitionP(arr, lo, hi, key, asc);
    quickSortRec(arr, lo, p - 1, key, asc);
    quickSortRec(arr, p + 1, hi, key, asc);
}
void quickSort(Product* arr, int n, int key, bool asc) {
    if (n > 1) quickSortRec(arr, 0, n - 1, key, asc);
}

// ---------------------------------------------------------------------------
// LAB 6: Searching
// ---------------------------------------------------------------------------
// Linear search: partial, case-insensitive match on name (prints all matches)
int linearSearchByName(const Product* arr, int n, const string& keyword) {
    string k = toLower(keyword);
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (toLower(arr[i].name).find(k) != string::npos) {
            if (found == 0) printHeader();
            printProduct(arr[i]);
            found++;
        }
    }
    return found;
}

// Binary search: array MUST be sorted by id ascending
int binarySearchById(const Product* arr, int n, int id) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid].id == id) return mid;
        if (arr[mid].id < id) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// ---------------------------------------------------------------------------
// Feature functions
// ---------------------------------------------------------------------------
void seedData(Inventory& inv) {
    Product seed[] = {
        {101, "iPhone 15 128GB",         "Smartphone", "Apple",   45990.00, 12},
        {102, "MacBook Air M2",          "Laptop",     "Apple",   62990.00,  6},
        {103, "AirPods Pro 2",           "Audio",      "Apple",   14990.00, 20},
        {201, "Galaxy S24 256GB",        "Smartphone", "Samsung", 48990.00, 10},
        {202, "Galaxy Tab S9",           "Tablet",     "Samsung", 39990.00,  8},
        {203, "Galaxy Buds2 Pro",        "Audio",      "Samsung",  8990.00, 25},
        {301, "WH-1000XM5 Headphones",   "Audio",      "Sony",    21990.00, 15},
        {302, "PlayStation 5 Slim",      "Console",    "Sony",    32990.00,  7},
        {303, "Alpha A7 IV Camera",      "Camera",     "Sony",   129990.00,  3},
        {401, "ROG Zephyrus G14",        "Laptop",     "Asus",    99990.00,  4},
        {402, "Zenbook 14 OLED",         "Laptop",     "Asus",    54990.00,  9},
        {403, "ROG Phone 8",             "Smartphone", "Asus",    59990.00,  5},
    };
    for (size_t i = 0; i < sizeof(seed) / sizeof(seed[0]); i++) inv.addProduct(seed[i]);
}

void displayAll(const Inventory& inv) {
    if (inv.size() == 0) { cout << "Inventory is empty.\n"; return; }
    printHeader();
    for (ListNode* c = inv.first(); c; c = c->next) printProduct(c->data);
    cout << "Total products: " << inv.size() << "\n";
}

void filterByBrand(const Inventory& inv) {
    string brand = toLower(readText("Enter brand (Apple/Samsung/Sony/Asus): "));
    bool any = false;
    for (ListNode* c = inv.first(); c; c = c->next) {
        if (toLower(c->data.brand) == brand) {
            if (!any) printHeader();
            printProduct(c->data);
            any = true;
        }
    }
    if (!any) cout << "No products found for that brand.\n";
}

void searchByName(const Inventory& inv) {
    string kw = readText("Enter product name keyword: ");
    int n = 0;
    Product* arr = inv.toArray(n);
    if (linearSearchByName(arr, n, kw) == 0) cout << "No matching products.\n";
    delete[] arr;
}

void searchByIdBST(const Inventory& inv) {
    int id = readInt("Enter product ID: ", 1, 999999);
    Product* p = inv.findById(id);
    if (p) { printHeader(); printProduct(*p); }
    else cout << "Product ID " << id << " not found (BST search).\n";
}

void searchByIdBinary(const Inventory& inv) {
    int id = readInt("Enter product ID: ", 1, 999999);
    int n = 0;
    Product* arr = inv.toArray(n);
    if (n == 0) { cout << "Inventory is empty.\n"; return; }
    mergeSort(arr, n, 4, true);  // binary search needs sorted-by-id array
    int idx = binarySearchById(arr, n, id);
    if (idx >= 0) { printHeader(); printProduct(arr[idx]); cout << "(found at sorted index " << idx << ")\n"; }
    else cout << "Product ID " << id << " not found (binary search).\n";
    delete[] arr;
}

void sortAndDisplay(const Inventory& inv) {
    int n = 0;
    Product* arr = inv.toArray(n);
    if (n == 0) { cout << "Inventory is empty.\n"; return; }
    cout << "Sort by:  1) Price  2) Name  3) Stock  4) ID\n";
    int key = readInt("Choose key: ", 1, 4);
    cout << "Algorithm: 1) Bubble  2) Selection  3) Insertion  4) Merge  5) Quick\n";
    int algo = readInt("Choose algorithm: ", 1, 5);
    int dir = readInt("Order: 1) Ascending  2) Descending: ", 1, 2);
    bool asc = (dir == 1);
    const char* names[] = {"", "Bubble Sort", "Selection Sort", "Insertion Sort", "Merge Sort", "Quick Sort"};
    switch (algo) {
        case 1: bubbleSort(arr, n, key, asc); break;
        case 2: selectionSort(arr, n, key, asc); break;
        case 3: insertionSort(arr, n, key, asc); break;
        case 4: mergeSort(arr, n, key, asc); break;
        default: quickSort(arr, n, key, asc); break;
    }
    cout << "Sorted using " << names[algo] << ":\n";
    printHeader();
    for (int i = 0; i < n; i++) printProduct(arr[i]);
    delete[] arr;
}

void placeOrder(const Inventory& inv, OrderQueue& q, int& nextOrderId) {
    string customer = readText("Customer name: ");
    int pid = readInt("Product ID: ", 1, 999999);
    Product* p = inv.findById(pid);
    if (!p) { cout << "Product not found. Order cancelled.\n"; return; }
    cout << "Selected: " << p->name << " (" << p->brand << ") - stock " << p->stockQuantity << "\n";
    int qty = readInt("Quantity: ", 1, 100000);
    Order o = {nextOrderId++, customer, pid, qty};
    q.enqueue(o);
    cout << "Order " << o.orderId << " added to the checkout queue (position " << q.size() << ").\n";
}

void processNextOrder(Inventory& inv, OrderQueue& q) {
    Order o;
    if (!q.dequeue(o)) { cout << "No pending orders.\n"; return; }
    Product* p = inv.findById(o.productId);
    cout << "Processing Order " << o.orderId << " for " << o.customer << "...\n";
    if (!p) { cout << "  FAILED: product no longer exists.\n"; return; }
    if (p->stockQuantity < o.quantity) {
        cout << "  FAILED: insufficient stock for " << p->name << " (requested " << o.quantity
             << ", available " << p->stockQuantity << ").\n";
        return;
    }
    p->stockQuantity -= o.quantity;
    cout << "  SUCCESS: " << o.quantity << " x " << p->name << " = PHP " << fixed << setprecision(2)
         << p->price * o.quantity << "  (remaining stock: " << p->stockQuantity << ")\n";
}

void adminAdd(Inventory& inv, UndoStack& undo) {
    Product p;
    p.id = inv.maxId() + 1;
    p.name = readText("Product name: ");
    p.category = readText("Category: ");
    cout << "Partner brands: Apple, Samsung, Sony, Asus\n";
    while (true) {
        string b = toLower(readText("Brand: "));
        if (b == "apple") p.brand = "Apple";
        else if (b == "samsung") p.brand = "Samsung";
        else if (b == "sony") p.brand = "Sony";
        else if (b == "asus") p.brand = "Asus";
        else { cout << "  [!] Not an official partner brand.\n"; continue; }
        break;
    }
    p.price = readPrice("Price (PHP): ");
    p.stockQuantity = readInt("Stock quantity: ", 0, 1000000);
    if (inv.addProduct(p)) {
        Action a = {ACT_ADD, p, 0};
        undo.push(a);
        cout << "Product added with ID " << p.id << ".\n";
    }
}

void adminRemove(Inventory& inv, UndoStack& undo) {
    int id = readInt("Product ID to remove: ", 1, 999999);
    Product removed;
    if (inv.removeProduct(id, &removed)) {
        Action a = {ACT_REMOVE, removed, 0};
        undo.push(a);
        cout << "Removed: " << removed.name << ".\n";
    } else cout << "Product not found.\n";
}

void adminRestock(Inventory& inv, UndoStack& undo) {
    int id = readInt("Product ID to restock: ", 1, 999999);
    Product* p = inv.findById(id);
    if (!p) { cout << "Product not found.\n"; return; }
    cout << p->name << " current stock: " << p->stockQuantity << "\n";
    int add = readInt("Units to add: ", 1, 1000000);
    Action a = {ACT_RESTOCK, *p, p->stockQuantity};
    undo.push(a);
    p->stockQuantity += add;
    cout << "New stock: " << p->stockQuantity << "\n";
}

void undoLast(Inventory& inv, UndoStack& undo) {
    Action a;
    if (!undo.pop(a)) { cout << "Nothing to undo.\n"; return; }
    switch (a.type) {
        case ACT_ADD:
            inv.removeProduct(a.product.id);
            cout << "Undid ADD: removed " << a.product.name << ".\n";
            break;
        case ACT_REMOVE:
            inv.addProduct(a.product);
            cout << "Undid REMOVE: restored " << a.product.name << ".\n";
            break;
        case ACT_RESTOCK: {
            Product* p = inv.findById(a.product.id);
            if (p) { p->stockQuantity = a.oldStock; cout << "Undid RESTOCK: stock of " << p->name << " back to " << a.oldStock << ".\n"; }
            else cout << "Cannot undo restock: product no longer exists.\n";
            break;
        }
    }
}

void showMenu() {
    cout << "\n================ TECH & ELECTRONICS INVENTORY ================\n"
         << " Partner Brands: Apple | Samsung | Sony | Asus\n"
         << "--------------------------------------------------------------\n"
         << "  1) Display all products            (Linked List)\n"
         << "  2) Filter by brand\n"
         << "  3) Search by name                  (Linear Search)\n"
         << "  4) Search by ID                    (BST Search)\n"
         << "  5) Search by ID                    (Binary Search)\n"
         << "  6) Sort products                   (Bubble/Selection/Insertion/Merge/Quick)\n"
         << "  7) Place order                     (Queue - enqueue)\n"
         << "  8) Process next order              (Queue - dequeue, stock update)\n"
         << "  9) View pending orders\n"
         << " 10) Admin: Add product\n"
         << " 11) Admin: Remove product\n"
         << " 12) Admin: Restock product\n"
         << " 13) Admin: Undo last action         (Stack)\n"
         << " 14) View BST index (in-order)\n"
         << "  0) Exit\n"
         << "==============================================================\n";
}

int main() {
    Inventory inv;
    UndoStack undo;
    OrderQueue orders;
    int nextOrderId = 1001;
    seedData(inv);

    cout << "Welcome! " << inv.size() << " products loaded.\n";
    bool running = true;
    while (running) {
        showMenu();
        int choice = readInt("Select option: ", 0, 14);
        cout << "\n";
        switch (choice) {
            case 1: displayAll(inv); break;
            case 2: filterByBrand(inv); break;
            case 3: searchByName(inv); break;
            case 4: searchByIdBST(inv); break;
            case 5: searchByIdBinary(inv); break;
            case 6: sortAndDisplay(inv); break;
            case 7: placeOrder(inv, orders, nextOrderId); break;
            case 8: processNextOrder(inv, orders); break;
            case 9: orders.display(); break;
            case 10: adminAdd(inv, undo); break;
            case 11: adminRemove(inv, undo); break;
            case 12: adminRestock(inv, undo); break;
            case 13: undoLast(inv, undo); break;
            case 14: inv.printInorderIds(); break;
            case 0: running = false; break;
        }
        if (!cin) break;  // EOF safety
    }
    cout << "Goodbye! (All dynamic memory is released on exit.)\n";
    return 0;
}