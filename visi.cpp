
#include "visi.h"
#include "funkcijos.h"
#include <sstream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <numeric>

// ============ MIGLĖS HASH ============
namespace Migle {
    std::array<uint32_t, 8> bubble_sort_and_hash(std::vector<char>& arr, std::array<uint32_t, 8> seed) {
        int n = (int)arr.size();
        for (int i = 0; i < n - 1; ++i) {
            for (int j = 0; j < n - 1 - i; ++j) {
                if (arr[j] > arr[j+1]) {
                    unsigned int a = (unsigned char)arr[j];
                    unsigned int b = (unsigned char)arr[j+1];
                    int idx = j % 8;
                    seed[idx] = (seed[idx] << 5) + (seed[idx] >> 3) + (a * 17 + b * 31 + j * 13);
                    std::swap(arr[j], arr[j+1]);
                }
            }
        }
        return seed;
    }

    std::string hash_to_hex(const std::array<uint32_t, 8>& h) {
        std::ostringstream ss;
        ss << std::hex << std::setfill('0');
        for (uint32_t part : h) {
            ss << std::setw(8) << part;
        }
        return ss.str();
    }

    std::string hash(const std::string& input) {
        std::vector<char> data(input.begin(), input.end());
        std::array<uint32_t, 8> seed = {0x12345678, 0x9abcdef0, 0x11111111, 0x22222222,
                                         0x33333333, 0x44444444, 0x55555555, 0x66666666};
        seed = bubble_sort_and_hash(data, seed);
        return hash_to_hex(seed);
    }
}

// ============ JULIAUS HASH ============
namespace Juliaus {
    std::string hash(const std::string& str) {
        unsigned long long seed[8] = {0x5FAF3C1BULL, 0x6E8D3B27ULL, 0xA1C5E97FULL, 0x4B7D2E95ULL, 
                                       0xF2A39C68ULL, 0x3E9B5A7CULL, 0x9D74C5A1ULL, 0x7C1A5F3EULL};
        
        for (size_t i = 0; i < str.size(); ++i) {
            unsigned char cByte = str[i];
            size_t ind = i % 8;
            seed[ind] ^= ((seed[(ind + 1) % 8] << 7) | (seed[(ind + 7) % 8] >> 3));
            seed[ind] += cByte * 131 + (seed[(ind + 3) % 8] ^ seed[(ind + 5) % 8]);
        }

        for (int i = 0; i < 64; ++i) {
            size_t ind = i % 8;
            seed[ind] ^= ((seed[(ind + 1) % 8] << ((i * 7) % 61)) | (seed[(ind + 7) % 8] >> ((i * 5) % 53)));
            seed[ind] += (seed[(ind + 3) % 8] ^ seed[(ind + 5) % 8]) + (0x9E3779B97F4A7C15ULL ^ (i * 0xA1C52E95ULL));
        }

        unsigned long long out4[4];
        for (int i = 0; i < 4; ++i) {
            out4[i] = seed[i] ^ (seed[i + 4] << 1) ^ (seed[(i + 2) % 8] >> 1);
        }

        char out[65];
        snprintf(out, sizeof(out), "%016llx%016llx%016llx%016llx", out4[0], out4[1], out4[2], out4[3]);
        return std::string(out);
    }
}

// ============ TĖJOS HASH ============
namespace Tejos {
    const uint64_t INIT_A = 0x428a2f98d728ae22ULL;
    const uint64_t INIT_B = 0x7137449123ef65cdULL;
    const uint64_t INIT_C = 0xb5c0fbcfec4d3b2fULL;
    const uint64_t INIT_D = 0xe9b5dba58189dbbcULL;
    const uint64_t PRIME1 = 0x9e3779b185ebca87ULL;
    const uint64_t PRIME2 = 0xc2b2ae3d27d4eb4fULL;
    const uint64_t PRIME3 = 0x165667b19e3779f9ULL;
    const uint64_t PRIME4 = 0x85ebca77c2b2ae63ULL;
    
    uint64_t rotateLeft(uint64_t value, int shift) {
        return (value << shift) | (value >> (64 - shift));
    }
    
    uint64_t rotateRight(uint64_t value, int shift) {
        return (value >> shift) | (value << (64 - shift));
    }
    
    uint64_t mix(uint64_t a, uint64_t b, uint64_t c) {
        a ^= b;
        a = rotateLeft(a, 23);
        a *= PRIME1;
        a ^= c;
        a = rotateLeft(a, 41);
        return a;
    }

    std::string hash(const std::string& input) {
        uint64_t h1 = INIT_A, h2 = INIT_B, h3 = INIT_C, h4 = INIT_D;
        size_t len = input.length();
        
        for (size_t i = 0; i < len; i++) {
            uint64_t byte_val = static_cast<uint64_t>(static_cast<unsigned char>(input[i]));
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
            
            if (i % 4 == 3) {
                h1 = mix(h1, h2, h3);
                h2 = mix(h2, h3, h4);
                h3 = mix(h3, h4, h1);
                h4 = mix(h4, h1, h2);
            }
        }
        
        h1 ^= len * PRIME1;
        h2 ^= len * PRIME2;
        h3 ^= len * PRIME3;
        h4 ^= len * PRIME4;
        
        h1 = mix(h1, h2, h3);
        h2 = mix(h2, h3, h4);
        h3 = mix(h3, h4, h1);
        h4 = mix(h4, h1, h2);
        
        h1 += h2 + h3 + h4;
        h2 += h1;
        h3 += h1;
        h4 += h1;
        
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        ss << std::setw(16) << h1 << std::setw(16) << h2 << std::setw(16) << h3 << std::setw(16) << h4;
        return ss.str();
    }
}

// ============ GUSTAVO HASH ============
namespace Gustavo {
    std::string wordToBinary(const std::string& text) {
        std::string binary;
        for (unsigned char c : text) {
            binary += std::bitset<8>(c).to_string();
        }
        return binary;
    }

    char fourNumbersToHex(const std::string& nibble) {
        int value = std::bitset<4>(nibble).to_ulong();
        if (value < 10) return '0' + value;
        return 'A' + (value - 10);
    }

    std::string binaryToHex(const std::string& binary) {
        std::string hex;
        for (size_t i = 0; i < binary.size(); i += 4) {
            hex += fourNumbersToHex(binary.substr(i, 4));
        }
        return hex;
    }

    std::string hash(std::string input) {
        int seed = (input.length() % 10) + 9;
        input = wordToBinary(input);
        while (input.length() < 32) {
            input += input;
            input += "1011101";
        }
        
        int amount_of_1 = 0;
        int amount_of_0 = 0;
        for (size_t i = 0; i < input.length(); i++) {
            if (input[i] == '1')
                amount_of_1 += i;
            else
                amount_of_0 += i;
        }
        
        if (amount_of_1 < 0) amount_of_1 *= -1;
        if (amount_of_0 < 0) amount_of_0 *= -1;
        
        int bigger = ((amount_of_0 > amount_of_1) ? amount_of_0 : amount_of_1) * seed;
        int smaller = ((amount_of_0 < amount_of_1) ? amount_of_0 : amount_of_1);
        if ((bigger % smaller) == 0)
            smaller++;
        
        if (bigger < 0) bigger *= -1;
        if (smaller < 0) smaller *= -1;
        
        if (bigger > 100000) {
            bigger = bigger % 1000000 + 1000000;
            smaller = smaller % 1000000 + 100000;
        }
        
        while (input.size() < 256) {
            input += input;
        }
        
        int current = 0;
        for (int i = 0; i < bigger; i++) {
            char temp = input[current];
            int next_pos = (current + smaller - i) % input.length();
            input[current] = input[next_pos];
            input[next_pos] = temp;
            current = next_pos;
        }
        
        input = binaryToHex(input);
        input = wordToBinary(input);
        
        current = 0;
        for (int i = 0; i < bigger; i++) {
            char temp = input[current];
            int next_pos = (current + smaller - i) % input.length();
            input[current] = input[next_pos];
            input[next_pos] = temp;
            current = next_pos;
        }
        
        std::string Hashed(256, '\0');
        for (int i = 0; i < 256; i++) {
            Hashed[i] = input[i];
        }
        
        return binaryToHex(Hashed);
    }
}

// ============ NEDOS HASH ============
namespace Nedos {
    const char BASE62[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    inline char to_base62(int sk) {
        sk %= 62;
        if (sk < 0) sk += 62;
        return BASE62[sk];
    }

    void value_dependent_shuffle(std::vector<int>& previous) {
        if (previous.empty()) return;
        std::vector<int> temp = previous;
        const size_t n = previous.size();
        
        for (size_t i = 0; i < n; ++i) {
            int value = temp[i];
            size_t new_pos;
            
            if (value % 2 == 0) {
                size_t jump = (17 * abs(value) * 7 + i * 23) % n;
                new_pos = (i + jump) % n;
            } else {
                size_t jump = (13 * abs(value) * 11 + i * 19) % n;
                new_pos = (i + n - (jump % n)) % n;
            }
            previous[new_pos] = value;
        }
    }

    void swap_halves(std::vector<int>& previous) {
        size_t n = previous.size();
        size_t h = n / 2;
        std::vector<int> first(previous.begin(), previous.begin() + h);
        std::vector<int> second(previous.begin() + h, previous.end());
        previous.clear();
        previous.insert(previous.end(), second.begin(), second.end());
        previous.insert(previous.end(), first.begin(), first.end());
    }

    void three_in_one_mixer(std::vector<int>& previous) {
        if (previous.empty()) return;
        std::vector<int> temp = previous;
        
        for (size_t i = 0; i < temp.size(); ++i) {
            int sk = temp[i];
            size_t e1 = (i + sk) % previous.size();
            size_t e2 = (i + sk * 2) % previous.size();
            size_t e3 = (i + sk * 3) % previous.size();
            
            previous[e1] = (previous[e1] + sk) % 256;
            previous[e2] = (previous[e2] + sk * 2) % 256;
            previous[e3] = (previous[e3] + sk * 3) % 256;
        }
    }

    std::string generate_seed(const std::vector<int>& current) {
        std::string seed = "Kx9mN3vL8qR5wY1pZ7jT2bF6hC4nA0sD";
        int matrix[2][2] = {{7, 13}, {11, 5}};
        
        for (size_t i = 0; i + 1 < current.size(); i += 2) {
            int a = current[i];
            int b = current[i + 1];
            size_t pos1 = i % seed.size();
            size_t pos2 = (i + 1) % seed.size();
            seed[pos1] = BASE62[(matrix[0][0] * a + matrix[0][1] * b) % 62];
            seed[pos2] = BASE62[(matrix[1][0] * a + matrix[1][1] * b) % 62];
        }
        
        if (current.size() % 2 == 1) {
            size_t last_idx = current.size() - 1;
            size_t pos = last_idx % seed.size();
            seed[pos] = BASE62[(current[last_idx] * 17 + pos * 23) % 62];
        }
        return seed;
    }

    std::string hash(const std::string& user_input) {
        std::vector<int> current;
        current.reserve(user_input.size());
        for (unsigned char c : user_input) current.push_back((int)c);
        if (current.empty()) current.push_back(0);
        
        constexpr int ROUNDS = 4;
        std::vector<int> data = current;
        std::vector<int> previous;
        
        for (int r = 0; r < ROUNDS; ++r) {
            previous = data;
            value_dependent_shuffle(previous);
            three_in_one_mixer(previous);
            swap_halves(previous);
            if (previous.empty()) previous.push_back(0);
            data = previous;
        }
        
        std::string seed = generate_seed(current);
        
        std::string salt;
        if (!current.empty()) {
            size_t input_sum = 0;
            for (int ascii : current) input_sum += ascii;
            
            for (int i = 0; i < 4; ++i) {
                int salt_62 = (input_sum * (i + 7) + current[i % current.size()] * 13) % 62;
                salt += to_base62(salt_62);
            }
            
            for (size_t i = 0; i < salt.size(); ++i) {
                int salt_ascii = (int)(unsigned char)salt[i];
                size_t salt_pos = (input_sum + i * salt_ascii) % seed.size();
                int seed_62 = (seed[salt_pos] + salt_ascii + input_sum) % 62;
                seed[salt_pos] = to_base62(seed_62);
            }
        }
        
        for (size_t i = 0; i < previous.size(); ++i) {
            size_t si = i % seed.size();
            int rez = ((int)(unsigned char)seed[si] * previous[i] * (i + 1)) % 256;
            seed[si] = to_base62(rez % 62);
        }
        
        std::string out;
        out.reserve(64);
        for (int i = 0; i < 64; ++i) {
            int a = (int)(unsigned char)seed[i % seed.size()];
            int b = previous[i % previous.size()];
            int v = (a + b + i * 17) % 62;
            out.push_back(to_base62(v));
        }
        return out;
    }
}

// ============ MANO HASH ============
namespace Mano {
    std::map<wchar_t, uint16_t> getLithuanianCharMap() {
        std::map<wchar_t, uint16_t> charMap;
        charMap[L'ą'] = 0xC485; charMap[L'č'] = 0xC48D; charMap[L'ę'] = 0xC499;
        charMap[L'ė'] = 0xC497; charMap[L'į'] = 0xC4AF; charMap[L'š'] = 0xC5A1;
        charMap[L'ų'] = 0xC5B3; charMap[L'ū'] = 0xC5AB; charMap[L'ž'] = 0xC5BE;
        charMap[L'Ą'] = 0xC484; charMap[L'Č'] = 0xC48C; charMap[L'Ę'] = 0xC498;
        charMap[L'Ė'] = 0xC496; charMap[L'Į'] = 0xC4AE; charMap[L'Š'] = 0xC5A0;
        charMap[L'Ų'] = 0xC5B2; charMap[L'Ū'] = 0xC5AA; charMap[L'Ž'] = 0xC5BD;
        return charMap;
    }

    std::string convertLithuanianText(const std::string &input) {
        static auto charMap = getLithuanianCharMap();
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring wide = conv.from_bytes(input);
        std::string result;
        
        for (wchar_t wc : wide) {
            auto it = charMap.find(wc);
            if (it != charMap.end()) {
                uint16_t code = it->second;
                result.push_back(static_cast<char>((code >> 8) & 0xFF));
                result.push_back(static_cast<char>(code & 0xFF));
            } else {
                std::string utf8char = conv.to_bytes(wc);
                result += utf8char;
            }
        }
        return result;
    }

    uint32_t safeStringToUint32(const std::string& str, const std::string& seedui) {
        std::string truncated = str;
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

    std::string hash(const std::string &ivestis) {
        std::string isvestis;
        std::string konvertuotasIvestis = convertLithuanianText(ivestis);
        
        std::string seedString;
        std::string seedui;
        if (!konvertuotasIvestis.empty()) {
            for (size_t i = 0; i < konvertuotasIvestis.size(); i += 1000) {
                int suma = 0;
                for (size_t j = i; j < i + 10 && j < konvertuotasIvestis.size(); j++) {
                    suma += static_cast<unsigned char>(konvertuotasIvestis[j]);
                }
                seedString += std::to_string(suma);
            }
            
            for (size_t i = 0; i < konvertuotasIvestis.size(); i += 20) {
                int ones = 0;
                for (size_t j = i; j < i + 20 && j < konvertuotasIvestis.size(); j++) {
                    std::bitset<8> bits(static_cast<unsigned char>(konvertuotasIvestis[j]));
                    ones += bits.count();
                }
                seedString += std::to_string(ones);
                seedui += std::to_string(ones);
            }
        } else {
            seedString = "0";
        }
        
        std::string binaryInput;
        for (unsigned char c : konvertuotasIvestis) {
            binaryInput += std::bitset<8>(c).to_string();
        }
        
        if (binaryInput.empty()) {
            binaryInput = "10000000";
        }
        
        std::string originalBinary = binaryInput;
        while (binaryInput.size() < 256) {
            std::string toAdd = originalBinary;
            for (size_t i = 0; i < toAdd.size() && binaryInput.size() < 256; i++) {
                char newBit = (binaryInput[i % binaryInput.size()] == toAdd[i]) ? '0' : '1';
                binaryInput += newBit;
            }
        }
        if (binaryInput.size() > 256) {
            binaryInput = binaryInput.substr(0, 256);
        }
        
        uint32_t mySeed = safeStringToUint32(seedString, seedui);
        std::mt19937 rng(mySeed);
        
        std::string mixedBinary;
        for (size_t i = 0; i < binaryInput.size(); i += 32) {
            uint32_t randVal = rng();
            for (size_t j = 0; j < 32 && i + j < binaryInput.size(); j++) {
                int bit = binaryInput[i + j] - '0';
                int rbit = (randVal >> j) & 1;
                bit ^= rbit;
                mixedBinary.push_back(bit ? '1' : '0');
            }
        }
        
        for (size_t i = 0; i + 4 <= mixedBinary.size(); i += 4) {
            std::string nibble = mixedBinary.substr(i, 4);
            int value = std::stoi(nibble, nullptr, 2);
            std::stringstream ss;
            ss << std::hex << value;
            isvestis += ss.str();
        }
        return isvestis;
    }
}

// ============ KOMBINUOTAS HASH ============
namespace Visi {
    std::string hash(const std::string& input) {
        return ::hashas(input);
    }
}