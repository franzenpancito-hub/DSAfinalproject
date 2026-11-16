// ============================================================================
//  ProductBST.h - LAB 6: Binary Search Tree indexed by Product ID
// ============================================================================
#ifndef PRODUCTBST_H
#define PRODUCTBST_H

#include "Catalog.h"

struct BSTNode {
    int id;
    int index;          // position inside the Catalog array
    BSTNode* left;
    BSTNode* right;
};

class ProductBST {
private:
    BSTNode* root;

    BSTNode* insertNode(BSTNode* node, int id, int index);
    int searchNode(BSTNode* node, int id, int& steps) const;
    void destroy(BSTNode* node);
    void buildBalanced(const Catalog& c, int lo, int hi);

public:
    ProductBST();
    ~ProductBST();
    ProductBST(const ProductBST&) = delete;
    ProductBST& operator=(const ProductBST&) = delete;

    // Catalog must already be sorted by ID (seed data is).
    void buildFrom(const Catalog& c);

    // Returns catalog index, or -1 if not found. steps = nodes visited.
    int search(int id, int& steps) const;
};

#endif
