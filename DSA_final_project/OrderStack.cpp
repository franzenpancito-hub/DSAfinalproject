// ============================================================================
//  OrderStack.cpp - LAB 4
// ============================================================================
#include "OrderStack.h"

OrderStack::OrderStack() : top(nullptr), count(0) {}
OrderStack::~OrderStack() { clear(); }

void OrderStack::push(const Order& o) {
    top = new StackNode{o, top};
    count++;
}

bool OrderStack::pop(Order& out) {
    if (top == nullptr) return false;
    StackNode* old = top;
    out = old->order;
    top = old->next;
    delete old;
    count--;
    return true;
}

void OrderStack::clear() { Order tmp; while (pop(tmp)) {} }
