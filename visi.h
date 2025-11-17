#ifndef VISI_H
#define VISI_H

#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <map>
#include <random>
#include <locale>
#include <codecvt>

// Miglės hash funkcija
namespace Migle {
    std::array<uint32_t, 8> bubble_sort_and_hash(std::vector<char>& arr, std:: array<uint32_t, 8> seed);
    std::string hash_to_hex(const std::array<uint32_t, 8>& h);
    std::string hash(const std::string& input);
}

// Juliaus hash funkcija
namespace Juliaus {
    std::string hash(const std::string& str);
}

// Tėjos hash funkcija
namespace Tejos {
    extern const uint64_t INIT_A;
    extern const uint64_t INIT_B;
    extern const uint64_t INIT_C;
    extern const uint64_t INIT_D;
    extern const uint64_t PRIME1;
    extern const uint64_t PRIME2;
    extern const uint64_t PRIME3;
    extern const uint64_t PRIME4;
    
    uint64_t rotateLeft(uint64_t value, int shift);
    uint64_t rotateRight(uint64_t value, int shift);
    uint64_t mix(uint64_t a, uint64_t b, uint64_t c);
    std::string hash(const std::string& input);
}

// Gustavo hash funkcija
namespace Gustavo {
    std::string hash(std::string input);
}

// Nedos hash funkcija
namespace Nedos {
    extern const char BASE62[];
    void value_dependent_shuffle(std::vector<int>& previous);
    void swap_halves(std::vector<int>& previous);
    void three_in_one_mixer(std::vector<int>& previous);
    std::string generate_seed(const std::vector<int>& current);
    std::string hash(const std::string& user_input);
}

// Mano  hash funkcija
namespace Mano {
    std::map<wchar_t, uint16_t> getLithuanianCharMap();
    std::string convertLithuanianText(const std::string &input);
    uint32_t safeStringToUint32(const std::string& str, const std::string& seedui);
    std::string hash(const std::string &ivestis);
}

// Kombinuota hash funkcija
namespace Visi {
    std::string hash(const std::string& input);
}

#endif // VISI_H