// ============================================================================
//  Utils.cpp
// ============================================================================
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cctype>
using namespace std;

string toLower(string s) {
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

int readInt(const string& prompt, int lo, int hi) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (v >= lo && v <= hi) return v;
            cout << "  Invalid: enter a number from " << lo << " to " << hi << ".\n";
        } else {
            if (cin.eof()) throw InputClosed();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input. Numbers only.\n";
        }
    }
}

double readDouble(const string& prompt, double lo, double hi) {
    while (true) {
        cout << prompt;
        double v;
        if (cin >> v) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (v >= lo && v <= hi) return v;
            cout << "  Invalid: enter an amount from " << lo << " to " << hi << ".\n";
        } else {
            if (cin.eof()) throw InputClosed();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input. Numbers only.\n";
        }
    }
}

string readLine(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) throw InputClosed();
    return s;
}

void printLine(char c, int n) {
    cout << string(n, c) << "\n";
}

void printProductHeader() {
    printLine();
    cout << left << setw(6) << "ID" << setw(30) << "Product" << setw(13) << "Category"
         << setw(10) << "Brand" << right << setw(13) << "Price (PHP)" << setw(7) << "Stock" << "\n";
    printLine();
}

void printProductRow(const Product& p) {
    cout << left << setw(6) << p.id << setw(30) << p.name << setw(13) << p.category
         << setw(10) << p.brand << right << setw(13) << p.price << setw(7)
         << (p.stockQuantity == 0 ? string("OUT") : to_string(p.stockQuantity)) << "\n";
}
