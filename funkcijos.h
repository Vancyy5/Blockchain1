#ifndef FUNKCIJOS_H      
#define FUNKCIJOS_H

#include "lib.h" 

string convertLithuanianText(const string &input); 
void hashas (const string &ivestis, string &isvestis);
void initAllChars(); 
string generateRandomString(size_t length, mt19937 &rng);
void createSingleCharFiles();
void createLargeRandomFiles();
void createSimilarFiles();
void generateCollisionTestPairs();  
void generateAvalancheTestPairs(); 

#endif // FUNKCIJOS_H