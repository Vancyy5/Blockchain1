#ifndef TESTAVIMAS_H
#define TESTAVIMAS_H

#include "lib.h"
#include "laikas.h"
#include "funkcijos.h"
#include "visi.h"

// Išorinė hash wrapper funkcija (apibrėžta main.cpp)
extern void hashWrapper(const string& input, string& output);

// Efektyvumo testavimas
void testEfficiency();
void measureHashingTime(const string& content, int lineMultiplier, vector<double>& times);
void createEfficiencyGraph(const vector<int>& lineCounts, const vector<double>& avgTimes);

// Kolizijų paieška
void testCollisions();
void findCollisions(const vector<string>& strings, int length, int& collisions);

// Lavinos efektas
void testAvalancheEffect();
double calculateBitDifference(const string& hash1, const string& hash2);
double calculateHexDifference(const string& hash1, const string& hash2);

// Negrįžtamumo demonstracija
void testIrreversibility();
string addSalt(const string& input, const string& salt);
void demonstrateHiding(const string& message, const string& salt);
void demonstratePuzzleFriendliness();

// Pagalbinės funkcijos
string readConstitutionFile();
vector<string> splitIntoLines(const string& content);
void printStatistics(const vector<double>& values, const string& description);

#endif // TESTAVIMAS_H