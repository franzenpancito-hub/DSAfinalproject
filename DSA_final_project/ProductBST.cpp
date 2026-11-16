// ============================================================================
//  ProductBST.cpp - LAB 6
// ============================================================================
#include "ProductBST.h"

ProductBST::ProductBST() : root(nullptr) {}
ProductBST::~ProductBST() { destroy(root); }

BSTNode* ProductBST::insertNode(BSTNode* node, int id, int index) {
    if (node == nullptr) return new BSTNode{id, index, nullptr, nullptr};
    if (id < node->id) node->left = insertNode(node->left, id, index);
    else if (id > node->id) node->right = insertNode(node->right, id, index);
    return node;    // duplicate IDs are ignored (IDs must be unique)
}

int ProductBST::searchNode(BSTNode* node, int id, int& steps) const {
    if (node == nullptr) return -1;
    steps++;
    if (node->id == id) return node->index;
    if (id < node->id) return searchNode(node->left, id, steps);
    return searchNode(node->right, id, steps);
}

void ProductBST::destroy(BSTNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Insert middle first so the tree stays balanced -> O(log n) search.
void ProductBST::buildBalanced(const Catalog& c, int lo, int hi) {
    if (lo > hi) return;
    int mid = lo + (hi - lo) / 2;
    root = insertNode(root, c.at(mid).id, mid);
    buildBalanced(c, lo, mid - 1);
    buildBalanced(c, mid + 1, hi);
}

void ProductBST::buildFrom(const Catalog& c) { buildBalanced(c, 0, c.size() - 1); }

int ProductBST::search(int id, int& steps) const {
    steps = 0;
    return searchNode(root, id, steps);
}
