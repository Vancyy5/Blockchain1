#include "funkcijos.h"

map<wchar_t, uint16_t> getLithuanianCharMap() 
{
    map<wchar_t, uint16_t> charMap;

   
    charMap[L'ą'] = 0xC485;  
    charMap[L'č'] = 0xC48D;  
    charMap[L'ę'] = 0xC499;  
    charMap[L'ė'] = 0xC497;  
    charMap[L'į'] = 0xC4AF;  
    charMap[L'š'] = 0xC5A1;  
    charMap[L'ų'] = 0xC5B3;  
    charMap[L'ū'] = 0xC5AB;  
    charMap[L'ž'] = 0xC5BE;  

    
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

void hashas(const string &ivestis, string &isvestis) 
{
    isvestis.clear();
    string konvertuotasIvestis = convertLithuanianText(ivestis);
    
    // Konstantos
    const uint64_t PRIME1 = 0x9E3779B185EBCA87ULL;
    const uint64_t PRIME2 = 0xC2B2AE3D27D4EB4FULL;
    const uint64_t PRIME3 = 0x165667B19E3779F9ULL;
    const uint64_t PRIME4 = 0x85EBCA77C2B2AE63ULL;
    
    // Inicializuojame 512-bitų būseną (64 baitai)
    vector<uint64_t> state(8, 0);
    
    // 1. Pirminis seed iš įvesties charakteristikų
    uint64_t seed = PRIME1;
    
    if (!konvertuotasIvestis.empty()) {
        // Įvesties ilgio įtaka
        seed ^= konvertuotasIvestis.size() * PRIME2;
        
        // ASCII sumos su rotacija
        for (size_t i = 0; i < konvertuotasIvestis.size(); i++) {
            unsigned char c = konvertuotasIvestis[i];
            // Bit rotation inline
            seed = ((seed << 7) | (seed >> (64 - 7))) ^ (c * PRIME3);
        }
        
        // Pozicijos-priklausomas mixing
        for (size_t i = 0; i < konvertuotasIvestis.size(); i++) {
            size_t stateIdx = i % 8;
            state[stateIdx] ^= static_cast<uint64_t>(konvertuotasIvestis[i]) * (PRIME4 + i);
            // Rotation inline
            state[stateIdx] = (state[stateIdx] << 13) | (state[stateIdx] >> (64 - 13));
        }
    } else {
        seed = PRIME1;
        state[0] = PRIME2;
    }
    
    // 2. Avalanche mixing inline
    seed ^= seed >> 33;
    seed *= PRIME2;
    seed ^= seed >> 29;
    seed *= PRIME3;
    seed ^= seed >> 32;
    
    // 3. Inicializuojame RNG su pagerintu seed'u
    mt19937_64 rng(seed);
    
    // 4. Papildomas state mixing su RNG
    for (int round = 0; round < 4; round++) {
        for (size_t i = 0; i < 8; i++) {
            state[i] ^= rng();
            // Avalanche inline
            uint64_t h = state[i];
            h ^= h >> 33;
            h *= PRIME2;
            h ^= h >> 29;
            h *= PRIME3;
            h ^= h >> 32;
            state[i] = h;
        }
        
        // Sumaišome state'us tarpusavyje
        for (size_t i = 0; i < 8; i++) {
            size_t next = (i + 1) % 8;
            size_t prev = (i + 7) % 8;
            uint64_t rotNext = (state[next] << 17) | (state[next] >> (64 - 17));
            uint64_t rotPrev = (state[prev] << 31) | (state[prev] >> (64 - 31));
            state[i] ^= rotNext ^ rotPrev;
        }
    }
    
    // 5. Įvesties duomenų įterpimas į state
    string binaryInput;
    for (unsigned char c : konvertuotasIvestis) {
        binaryInput += bitset<8>(c).to_string();
    }
    
    if (binaryInput.empty()) {
        binaryInput = "10000000";
    }
    
    // 6. Pailginame įvestį su kompleksiniu algoritmu
    string originalBinary = binaryInput;
    size_t targetSize = 512;
    
    while (binaryInput.size() < targetSize) {
        for (size_t i = 0; i < originalBinary.size() && binaryInput.size() < targetSize; i++) {
            size_t pos1 = i % binaryInput.size();
            size_t pos2 = (i * 7) % originalBinary.size();
            size_t pos3 = (i * 13) % binaryInput.size();
            
            char bit1 = binaryInput[pos1];
            char bit2 = originalBinary[pos2];
            char bit3 = binaryInput[pos3];
            
            // 3-way XOR su majority function
            int sum = (bit1 - '0') + (bit2 - '0') + (bit3 - '0');
            char newBit = (sum >= 2) ? '1' : '0';
            binaryInput += newBit;
        }
    }
    
    binaryInput = binaryInput.substr(0, targetSize);
    
    // 7. Finalus mixing su RNG ir įvesties duomenimis
    string finalBinary;
    for (size_t i = 0; i < binaryInput.size(); i += 64) {
        uint64_t randVal = rng();
        uint64_t stateVal = state[(i / 64) % 8];
        
        for (size_t j = 0; j < 64 && i + j < binaryInput.size(); j++) {
            int inputBit = binaryInput[i + j] - '0';
            int randBit = (randVal >> j) & 1;
            int stateBit = (stateVal >> j) & 1;
            
            // 3-way XOR
            int finalBit = inputBit ^ randBit ^ stateBit;
            finalBinary.push_back(finalBit ? '1' : '0');
        }
    }
    
    // 8. Papildomas scrambling pass
    vector<int> permutation(finalBinary.size());
    for (size_t i = 0; i < permutation.size(); i++) {
        permutation[i] = i;
    }
    
    // Fisher-Yates shuffle su deterministiniu RNG
    mt19937 shuffleRng(seed ^ PRIME4);
    for (int i = permutation.size() - 1; i > 0; i--) {
        int j = shuffleRng() % (i + 1);
        swap(permutation[i], permutation[j]);
    }
    
    string scrambledBinary;
    for (size_t i = 0; i < finalBinary.size(); i++) {
        scrambledBinary.push_back(finalBinary[permutation[i]]);
    }
    
    // 9. Konvertuojame į HEX (256 bitų = 64 hex simboliai)
    for (size_t i = 0; i < 256; i += 4) {
        string nibble = scrambledBinary.substr(i, 4);
        int value = stoi(nibble, nullptr, 2);
        stringstream ss;
        ss << hex << value;
        isvestis += ss.str();
    }
}

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