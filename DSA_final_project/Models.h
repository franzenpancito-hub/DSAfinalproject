// ============================================================================
//  Models.h - Plain data structures shared by every module
// ============================================================================
#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

struct Product {
    int id;                 // Unique Identifier
    std::string name;       // Product Title
    std::string category;   // Category (e.g., Smartphone, Audio)
    std::string brand;      // Official Partner Brand
    double price;           // Unit Price (PHP)
    int stockQuantity;      // Available Inventory Level
};

struct OrderItem {
    int productId;
    int qty;
};

struct Order {
    int orderNo;
    std::vector<OrderItem> items;
};

#endif
