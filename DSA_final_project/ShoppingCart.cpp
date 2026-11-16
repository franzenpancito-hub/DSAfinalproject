// ============================================================================
//  ShoppingCart.cpp - LAB 2
// ============================================================================
#include "ShoppingCart.h"

ShoppingCart::ShoppingCart() : head(nullptr), tail(nullptr), current(nullptr), count(0) {}
ShoppingCart::~ShoppingCart() { clear(); }

int ShoppingCart::qtyInCart(int id) const {
    for (CartNode* n = head; n != nullptr; n = n->next)
        if (n->productId == id) return n->qty;
    return 0;
}

void ShoppingCart::addItem(int id, int qty) {
    for (CartNode* n = head; n != nullptr; n = n->next) {
        if (n->productId == id) { n->qty += qty; return; }
    }
    CartNode* node = new CartNode{id, qty, tail, nullptr};
    if (tail == nullptr) head = node;
    else tail->next = node;
    tail = node;
    if (current == nullptr) current = node;
    count++;
}

void ShoppingCart::moveNext() { if (current && current->next) current = current->next; }
void ShoppingCart::movePrev() { if (current && current->prev) current = current->prev; }

void ShoppingCart::removeCurrent() {
    if (current == nullptr) return;
    CartNode* victim = current;
    if (victim->prev) victim->prev->next = victim->next; else head = victim->next;
    if (victim->next) victim->next->prev = victim->prev; else tail = victim->prev;
    current = victim->next ? victim->next : victim->prev;
    delete victim;
    count--;
}

void ShoppingCart::clear() {
    CartNode* n = head;
    while (n != nullptr) {
        CartNode* next = n->next;
        delete n;
        n = next;
    }
    head = tail = current = nullptr;
    count = 0;
}
