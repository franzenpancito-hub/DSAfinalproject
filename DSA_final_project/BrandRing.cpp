// ============================================================================
//  BrandRing.cpp - LAB 3
// ============================================================================
#include "BrandRing.h"
#include "Utils.h"
using namespace std;

BrandRing::BrandRing() : head(nullptr), tail(nullptr), count(0) {}

BrandRing::~BrandRing() {
    if (head == nullptr) return;
    tail->next = nullptr;               // break the circle before deleting
    BrandNode* n = head;
    while (n != nullptr) {
        BrandNode* next = n->next;
        delete n;
        n = next;
    }
}

void BrandRing::addBrand(const string& name) {
    BrandNode* n = head;
    for (int i = 0; i < count; i++, n = n->next)
        if (toLower(n->name) == toLower(name)) return;   // already listed
    BrandNode* node = new BrandNode{name, nullptr};
    if (head == nullptr) head = node;
    else tail->next = node;
    tail = node;
    tail->next = head;                  // always keep it circular
    count++;
}
