#ifndef FUNKCIJOS_H      
#define FUNKCIJOS_H

#include "lib.h" 

map<wchar_t, uint16_t> getLithuanianCharMap();
string convertLithuanianText(const string &input);


string hashas(const string& input);
void hashas(const string &ivestis, string &isvestis); 


uint32_t safeStringToUint32(const string& str, const string& seedui);
char to_base62(int sk);
uint64_t rotateLeft(uint64_t value, int shift);
uint64_t rotateRight(uint64_t value, int shift);
string wordToBinary(const string& text);
string binaryToHex(const string& binary);



#endif // FUNKCIJOS_H