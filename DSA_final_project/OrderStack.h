// ============================================================================
//  OrderStack.h - LAB 4: Stack (LIFO) - admin undo / redo
// ============================================================================
#ifndef ORDERSTACK_H
#define ORDERSTACK_H

#include "Models.h"

struct StackNode {
    Order order;
    StackNode* next;
};

class OrderStack {
private:
    StackNode* top;
    int count;

public:
    OrderStack();
    ~OrderStack();
    OrderStack(const OrderStack&) = delete;
    OrderStack& operator=(const OrderStack&) = delete;

    void push(const Order& o);
    bool pop(Order& out);
    void clear();

    bool empty() const { return top == nullptr; }
    int size() const { return count; }
};

#endif
