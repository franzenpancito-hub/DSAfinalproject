// ============================================================================
//  OrderQueue.cpp - LAB 5
// ============================================================================
#include "OrderQueue.h"

OrderQueue::OrderQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

OrderQueue::~OrderQueue() { Order tmp; while (dequeue(tmp)) {} }

void OrderQueue::enqueue(const Order& o) {
    QueueNode* node = new QueueNode{o, nullptr};
    if (rearNode == nullptr) frontNode = node;
    else rearNode->next = node;
    rearNode = node;
    count++;
}

bool OrderQueue::dequeue(Order& out) {
    if (frontNode == nullptr) return false;
    QueueNode* old = frontNode;
    out = old->order;
    frontNode = old->next;
    if (frontNode == nullptr) rearNode = nullptr;
    delete old;
    count--;
    return true;
}
