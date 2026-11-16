// ============================================================================
//  BrandRing.h - LAB 3: Circular linked list (brand banner showcase)
// ============================================================================
#ifndef BRANDRING_H
#define BRANDRING_H

#include <string>

struct BrandNode {
    std::string name;
    BrandNode* next;
};

class BrandRing {
private:
    BrandNode* head;
    BrandNode* tail;
    int count;

public:
    BrandRing();
    ~BrandRing();
    BrandRing(const BrandRing&) = delete;
    BrandRing& operator=(const BrandRing&) = delete;

    void addBrand(const std::string& name);
    int size() const { return count; }
    BrandNode* first() const { return head; }
};

#endif
