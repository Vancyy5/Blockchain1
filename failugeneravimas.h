#ifndef FAILUGENERAVIMAS_H      
#define FAILUGENERAVIMAS_H 

#include "lib.h" 

void initAllChars(); 
string generateRandomString(size_t length, mt19937 &rng);
void createSingleCharFiles();
void createLargeRandomFiles();
void createSimilarFiles();
void generateCollisionTestPairs();  
void generateAvalancheTestPairs(); 

#endif // FAILUGENERAVIMAS_H 