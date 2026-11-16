// ============================================================================
//  ShoppingCart.h - LAB 2: Doubly linked list (next/prev navigation)
// ============================================================================
#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

struct CartNode {
    int productId;
    int qty;
    CartNode* prev;
    CartNode* next;
};

class ShoppingCart {
private:
    CartNode* head;
    CartNode* tail;
    CartNode* current;      // the item the customer is "viewing"
    int count;

public:
    ShoppingCart();
    ~ShoppingCart();
    ShoppingCart(const ShoppingCart&) = delete;
    ShoppingCart& operator=(const ShoppingCart&) = delete;

    int qtyInCart(int id) const;
    void addItem(int id, int qty);
    void moveNext();
    void movePrev();
    void removeCurrent();
    void clear();

    bool empty() const { return head == nullptr; }
    int size() const { return count; }
    CartNode* first() const { return head; }
    CartNode* selected() const { return current; }
};

#endif
