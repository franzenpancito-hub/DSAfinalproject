// ============================================================================
//  Utils.h - Input validation, text helpers, table printing
// ============================================================================
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "Models.h"

struct InputClosed {};   // thrown on EOF so destructors still run (no leaks)

std::string toLower(std::string s);

int         readInt(const std::string& prompt, int lo, int hi);
double      readDouble(const std::string& prompt, double lo, double hi);
std::string readLine(const std::string& prompt);

void printLine(char c = '-', int n = 80);
void printProductHeader();
void printProductRow(const Product& p);

#endif
