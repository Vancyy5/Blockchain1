#ifndef FUNKCIJOS_H      
#define FUNKCIJOS_H

#include "lib.h" 

void hashas (const string &ivestis, string &isvestis);
void initAllChars(); 
string generateRandomString(size_t length, mt19937 &rng);
void createSingleCharFiles();
void createLargeRandomFiles();
void createSimilarFiles();

#endif // FUNKCIJOS_H