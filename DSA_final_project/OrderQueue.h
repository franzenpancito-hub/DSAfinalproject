// ============================================================================
//  OrderQueue.h - LAB 5: Queue (FIFO) - customer checkout line
// ============================================================================
#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include "Models.h"

struct QueueNode {
    Order order;
    QueueNode* next;
};

class OrderQueue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;
    int count;

public:
    OrderQueue();
    ~OrderQueue();
    OrderQueue(const OrderQueue&) = delete;
    OrderQueue& operator=(const OrderQueue&) = delete;

    void enqueue(const Order& o);
    bool dequeue(Order& out);

    bool empty() const { return frontNode == nullptr; }
    int size() const { return count; }
};

#endif
