#include "funkcijos.h"

map<wchar_t, uint16_t> getLithuanianCharMap() 
{
    map<wchar_t, uint16_t> charMap;
    
    // Mažosios raidės
    charMap[L'ą'] = 0xC485;  
    charMap[L'č'] = 0xC48D;  
    charMap[L'ę'] = 0xC499;  
    charMap[L'ė'] = 0xC497;  
    charMap[L'į'] = 0xC4AF;  
    charMap[L'š'] = 0xC5A1;  
    charMap[L'ų'] = 0xC5B3;  
    charMap[L'ū'] = 0xC5AB;  
    charMap[L'ž'] = 0xC5BE;  

    // Didžiosios raidės
    charMap[L'Ą'] = 0xC484;  
    charMap[L'Č'] = 0xC48C;  
    charMap[L'Ę'] = 0xC498;  
    charMap[L'Ė'] = 0xC496;  
    charMap[L'Į'] = 0xC4AE;  
    charMap[L'Š'] = 0xC5A0;  
    charMap[L'Ų'] = 0xC5B2;  
    charMap[L'Ū'] = 0xC5AA;  
    charMap[L'Ž'] = 0xC5BD;  

    return charMap;
}

string convertLithuanianText(const string &input) {
    static auto charMap = getLithuanianCharMap();
    wstring_convert<codecvt_utf8<wchar_t>> conv;
    wstring wide = conv.from_bytes(input);

    string result;

    for (wchar_t wc : wide) {
        auto it = charMap.find(wc);
        if (it != charMap.end()) {
            uint16_t code = it->second;
            result.push_back(static_cast<char>((code >> 8) & 0xFF));
            result.push_back(static_cast<char>(code & 0xFF));
        } else {
            string utf8char = conv.to_bytes(wc);
            result += utf8char;
        }
    }

    return result;
}

// Iš Nedos - base62 konvertavimas
const char BASE62[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

inline char to_base62(int sk) {
    sk %= 62;
    if (sk < 0) sk += 62;
    return BASE62[sk];
}

// Iš Tėjos - bit rotacijos funkcijos
uint64_t rotateLeft(uint64_t value, int shift) {
    return (value << shift) | (value >> (64 - shift));
}

uint64_t rotateRight(uint64_t value, int shift) {
    return (value >> shift) | (value << (64 - shift));
}

// Iš Gustavo - binary konversijos
string wordToBinary(const string& text) {
    string binary;
    for (unsigned char c : text) {
        binary += bitset<8>(c).to_string();
    }
    return binary;
}

string binaryToHex(const string& binary) {
    string hex;
    for (size_t i = 0; i + 4 <= binary.size(); i += 4) {
        int value = stoi(binary.substr(i, 4), nullptr, 2);
        stringstream ss;
        ss << std::hex << value;
        hex += ss.str();
    }
    return hex;
}

string hashas(const string& input) 
{
    // Konvertuojame lietuviškus simbolius
    string konvertuotasInput = convertLithuanianText(input);
    
    // 1. INICIALIZACIJA - iš Tėjos ir Juliaus
    // Tėjos konstantos
    const uint64_t INIT_A = 0x428a2f98d728ae22ULL;
    const uint64_t INIT_B = 0x7137449123ef65cdULL;
    const uint64_t INIT_C = 0xb5c0fbcfec4d3b2fULL;
    const uint64_t INIT_D = 0xe9b5dba58189dbbcULL;
    
    // Juliaus seed'as
    array<uint64_t, 8> seed = {
        0x5FAF3C1BULL, 0x6E8D3B27ULL, 0xA1C5E97FULL, 0x4B7D2E95ULL,
        0xF2A39C68ULL, 0x3E9B5A7CULL, 0x9D74C5A1ULL, 0x7C1A5F3EULL
    };
    
    uint64_t h1 = INIT_A, h2 = INIT_B, h3 = INIT_C, h4 = INIT_D;
    
    // 2. MIGLĖS - Bubble sort maisymas su hash'u
    vector<char> sortedInput(konvertuotasInput.begin(), konvertuotasInput.end());
    int n = sortedInput.size();
    
    for (int i = 0; i < min(n - 1, 100); ++i) { 
        for (int j = 0; j < n - 1 - i; ++j) {
            if (sortedInput[j] > sortedInput[j+1]) {
                unsigned int a = (unsigned char)sortedInput[j];
                unsigned int b = (unsigned char)sortedInput[j+1];
                
                int idx = j % 8;
                seed[idx] = (seed[idx] << 5) + (seed[idx] >> 3) + (a * 17 + b * 31 + j * 13);
                
                swap(sortedInput[j], sortedInput[j+1]);
            }
        }
    }
    
    // 3. JULIAUS - XOR ir shift operacijos
    for (size_t i = 0; i < konvertuotasInput.size(); ++i) {
        unsigned char cByte = konvertuotasInput[i];
        size_t ind = i % 8;
        seed[ind] ^= ((seed[(ind + 1) % 8] << 7) | (seed[(ind + 7) % 8] >> 3));
        seed[ind] += cByte * 131 + (seed[(ind + 3) % 8] ^ seed[(ind + 5) % 8]);
    }
    
    // 4. TĖJOS - Prime skaičiais maisymas su rotacijomis
    const uint64_t PRIME1 = 0x9e3779b185ebca87ULL;
    const uint64_t PRIME2 = 0xc2b2ae3d27d4eb4fULL;
    const uint64_t PRIME3 = 0x165667b19e3779f9ULL;
    const uint64_t PRIME4 = 0x85ebca77c2b2ae63ULL;
    
    for (size_t i = 0; i < konvertuotasInput.size(); ++i) {
        uint64_t byte_val = static_cast<uint64_t>(static_cast<unsigned char>(konvertuotasInput[i]));
        uint64_t position = i + 1;
        byte_val ^= position * PRIME1;
        
        h1 ^= byte_val * PRIME1;
        h1 = rotateLeft(h1, 13);
        h1 *= PRIME2;
        
        h2 ^= byte_val * PRIME2;
        h2 = rotateRight(h2, 17);
        h2 += h1;
        
        h3 ^= byte_val * PRIME3;
        h3 = rotateLeft(h3, 31);
        h3 ^= h2;
        
        h4 ^= byte_val * PRIME4;
        h4 = rotateRight(h4, 19);
        h4 += h3;
    }
    
    // 5. NEDOS - Value-dependent shuffle ir mixing
    vector<int> ascii_values;
    for (unsigned char c : konvertuotasInput) {
        ascii_values.push_back((int)c);
    }
    
    if (ascii_values.empty()) {
        ascii_values.push_back(0);
    }
    
    // Three-in-one mixer iš Nedos
    vector<int> temp = ascii_values;
    for (size_t i = 0; i < temp.size(); ++i) {
        int sk = temp[i];
        size_t e1 = (i + sk) % ascii_values.size();
        size_t e2 = (i + sk * 2) % ascii_values.size();
        size_t e3 = (i + sk * 3) % ascii_values.size();
        
        ascii_values[e1] = (ascii_values[e1] + sk) % 256;
        ascii_values[e2] = (ascii_values[e2] + sk * 2) % 256;
        ascii_values[e3] = (ascii_values[e3] + sk * 3) % 256;
    }
    
    // 6. Mano - Random number generator maisymas
    string seedString;
    for (size_t i = 0; i < konvertuotasInput.size(); i += 20) {
        int ones = 0;
        for (size_t j = i; j < i + 20 && j < konvertuotasInput.size(); j++) {
            bitset<8> bits(static_cast<unsigned char>(konvertuotasInput[j]));
            ones += bits.count();
        }
        seedString += to_string(ones);
    }
    
    uint32_t mySeed = 0;
    for (unsigned char c : seedString) {
        mySeed = mySeed * 31 + c;
    }
    mt19937 rng(mySeed);
    
    // 7. Gustavo - Binary permutacijos
    // pakeista kad priimtu kai tuscias
    string binaryData = wordToBinary(konvertuotasInput);
   if (binaryData.empty()) {
        binaryData = string(256, '0');
    } else {
        while (binaryData.size() < 256) {
            binaryData += binaryData;
        }
        binaryData = binaryData.substr(0, 256);
    }
    
    // Gustavo swap algoritmas
    int amount_of_1 = 0, amount_of_0 = 0;
    for (size_t i = 0; i < binaryData.length(); i++) {
        if (binaryData[i] == '1') amount_of_1 += i;
        else amount_of_0 += i;
    }
    
    int swaps = abs(amount_of_1 - amount_of_0) % 10000;
    int current = 0;
    int step = (konvertuotasInput.length() % 100) + 1;
    
    for (int i = 0; i < swaps; i++) {
        char temp_char = binaryData[current];
        int next_pos = (current + step) % binaryData.length();
        binaryData[current] = binaryData[next_pos];
        binaryData[next_pos] = temp_char;
        current = next_pos;
    }
    
    // 8. XOR su RNG iš mano
    string mixedBinary;
    for (size_t i = 0; i < binaryData.size(); i += 32) {
        uint32_t randVal = rng();
        for (size_t j = 0; j < 32 && i + j < binaryData.size(); j++) {
            int bit = binaryData[i + j] - '0';
            int rbit = (randVal >> j) & 1;
            bit ^= rbit;
            mixedBinary.push_back(bit ? '1' : '0');
        }
    }
    
    // 9. FINALIZACIJA - kombinuojame visus komponentus
    // Juliaus finalizacija
    for (int i = 0; i < 64; ++i) {
        size_t ind = i % 8;
        seed[ind] ^= ((seed[(ind + 1) % 8] << ((i * 7) % 61)) | 
                      (seed[(ind + 7) % 8] >> ((i * 5) % 53)));
        seed[ind] += (seed[(ind + 3) % 8] ^ seed[(ind + 5) % 8]);
    }
    
    // Tėjos finalizacija
    h1 ^= konvertuotasInput.length() * PRIME1;
    h2 ^= konvertuotasInput.length() * PRIME2;
    h3 ^= konvertuotasInput.length() * PRIME3;
    h4 ^= konvertuotasInput.length() * PRIME4;
    
    h1 += h2 + h3 + h4;
    h2 += h1;
    h3 += h1;
    h4 += h1;
    
    // 10. GALUTINIS HEX FORMAVIMAS
    stringstream result;
    result << hex << setfill('0');
    
    // Kombinuojame Juliaus seed'ą
    for (int i = 0; i < 4; ++i) {
        uint64_t combined = seed[i] ^ (seed[i + 4] << 1) ^ (seed[(i + 2) % 8] >> 1);
        result << setw(16) << combined;
    }
    
    // Pridedame Tėjos hash'ą
    result << setw(16) << h1 << setw(16) << h2 
           << setw(16) << h3 << setw(16) << h4;
    
    // Pridedame binary hash'ą iš Gustavo ir mano
    string hexFromBinary = binaryToHex(mixedBinary);
    
    // Kombinuojame viską į 64 simbolių hash'ą
    string fullHash = result.str() + hexFromBinary;
    
    // Paimame 64 simbolius naudojant Nedos seed principą
    string finalHash;
    
    for (int i = 0; i < 64; ++i) {
        size_t idx = (i * 17 + ascii_values[i % ascii_values.size()] * 23) % fullHash.size();
        finalHash += fullHash[idx];
    }
    
    return finalHash;
}

// Overload funkcija senai signatūrai (main.cpp naudoja)
void hashas(const string &ivestis, string &isvestis) 
{
    isvestis = hashas(ivestis);
}

// Pagalbinė funkcija (palikta backward compatibility)
uint32_t safeStringToUint32(const string& str, const string& seedui) 
{
    string truncated = str;
    if (truncated.length() > 9) {
        truncated = truncated.substr(0, 9);
    }
    
    uint32_t seed = 0;
    for (unsigned char c : seedui) {
        seed = seed * 31 + c; 
    }
    
    uint32_t hash = seed;
    for (unsigned char c : truncated) {
        hash = hash * seed + c; 
    }
    
    return hash;

}

