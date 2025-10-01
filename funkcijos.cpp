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

// ===== MD5 Implementation =====

inline uint32_t md5_F(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
inline uint32_t md5_G(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); }
inline uint32_t md5_H(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
inline uint32_t md5_I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }
inline uint32_t md5_rotateLeft(uint32_t x, uint32_t n) { return (x << n) | (x >> (32 - n)); }

void md5Hash(const string &input, string &output) {
    output.clear();
    
    // MD5 constants
    static const uint32_t s[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };
    
    static const uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
    
    // Initialize hash values
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;
    
    // Pre-processing
    string msg = input;
    uint64_t msgLen = msg.length() * 8;
    msg += (char)0x80;
    
    while ((msg.length() % 64) != 56) {
        msg += (char)0x00;
    }
    
    // Append length
    for (int i = 0; i < 8; i++) {
        msg += (char)((msgLen >> (i * 8)) & 0xFF);
    }
    
    // Process message in 512-bit chunks
    for (size_t offset = 0; offset < msg.length(); offset += 64) {
        uint32_t M[16];
        for (int i = 0; i < 16; i++) {
            M[i] = ((uint32_t)(unsigned char)msg[offset + i * 4 + 0]) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 1] << 8) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 2] << 16) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 3] << 24);
        }
        
        uint32_t A = a0, B = b0, C = c0, D = d0;
        
        for (int i = 0; i < 64; i++) {
            uint32_t F, g;
            if (i < 16) {
                F = md5_F(B, C, D);
                g = i;
            } else if (i < 32) {
                F = md5_G(B, C, D);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F = md5_H(B, C, D);
                g = (3 * i + 5) % 16;
            } else {
                F = md5_I(B, C, D);
                g = (7 * i) % 16;
            }
            
            F = F + A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + md5_rotateLeft(F, s[i]);
        }
        
        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    }
    
    // Produce final hash
    stringstream ss;
    ss << hex << setfill('0');
    ss << setw(2) << ((a0 >> 0) & 0xFF) << setw(2) << ((a0 >> 8) & 0xFF) 
       << setw(2) << ((a0 >> 16) & 0xFF) << setw(2) << ((a0 >> 24) & 0xFF);
    ss << setw(2) << ((b0 >> 0) & 0xFF) << setw(2) << ((b0 >> 8) & 0xFF) 
       << setw(2) << ((b0 >> 16) & 0xFF) << setw(2) << ((b0 >> 24) & 0xFF);
    ss << setw(2) << ((c0 >> 0) & 0xFF) << setw(2) << ((c0 >> 8) & 0xFF) 
       << setw(2) << ((c0 >> 16) & 0xFF) << setw(2) << ((c0 >> 24) & 0xFF);
    ss << setw(2) << ((d0 >> 0) & 0xFF) << setw(2) << ((d0 >> 8) & 0xFF) 
       << setw(2) << ((d0 >> 16) & 0xFF) << setw(2) << ((d0 >> 24) & 0xFF);
    
    output = ss.str();
}

// ===== SHA-1 Implementation =====

inline uint32_t sha1_rotateLeft(uint32_t value, uint32_t bits) {
    return (value << bits) | (value >> (32 - bits));
}

void sha1Hash(const string &input, string &output) {
    output.clear();
    
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;
    
    string msg = input;
    uint64_t msgLen = msg.length() * 8;
    msg += (char)0x80;
    
    while ((msg.length() % 64) != 56) {
        msg += (char)0x00;
    }
    
    // Append length (big-endian)
    for (int i = 7; i >= 0; i--) {
        msg += (char)((msgLen >> (i * 8)) & 0xFF);
    }
    
    // Process message in 512-bit chunks
    for (size_t offset = 0; offset < msg.length(); offset += 64) {
        uint32_t w[80];
        
        // Break chunk into sixteen 32-bit big-endian words
        for (int i = 0; i < 16; i++) {
            w[i] = ((uint32_t)(unsigned char)msg[offset + i * 4 + 0] << 24) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 1] << 16) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 2] << 8) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 3]);
        }
        
        // Extend the sixteen 32-bit words into eighty 32-bit words
        for (int i = 16; i < 80; i++) {
            w[i] = sha1_rotateLeft(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
        }
        
        uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;
        
        for (int i = 0; i < 80; i++) {
            uint32_t f, k;
            if (i < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (i < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (i < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }
            
            uint32_t temp = sha1_rotateLeft(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = sha1_rotateLeft(b, 30);
            b = a;
            a = temp;
        }
        
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }
    
    // Produce final hash (big-endian)
    stringstream ss;
    ss << hex << setfill('0');
    ss << setw(8) << h0 << setw(8) << h1 << setw(8) << h2 
       << setw(8) << h3 << setw(8) << h4;
    
    output = ss.str();
}

// ===== SHA-256 Implementation =====

static const uint32_t sha256_k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

inline uint32_t sha256_rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
inline uint32_t sha256_ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); }
inline uint32_t sha256_maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }
inline uint32_t sha256_Sigma0(uint32_t x) { return sha256_rotr(x, 2) ^ sha256_rotr(x, 13) ^ sha256_rotr(x, 22); }
inline uint32_t sha256_Sigma1(uint32_t x) { return sha256_rotr(x, 6) ^ sha256_rotr(x, 11) ^ sha256_rotr(x, 25); }
inline uint32_t sha256_sigma0(uint32_t x) { return sha256_rotr(x, 7) ^ sha256_rotr(x, 18) ^ (x >> 3); }
inline uint32_t sha256_sigma1(uint32_t x) { return sha256_rotr(x, 17) ^ sha256_rotr(x, 19) ^ (x >> 10); }

void sha256Hash(const string &input, string &output) {
    output.clear();
    
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    
    string msg = input;
    uint64_t msgLen = msg.length() * 8;
    msg += (char)0x80;
    
    while ((msg.length() % 64) != 56) {
        msg += (char)0x00;
    }
    
    // Append length (big-endian)
    for (int i = 7; i >= 0; i--) {
        msg += (char)((msgLen >> (i * 8)) & 0xFF);
    }
    
    // Process message in 512-bit chunks
    for (size_t offset = 0; offset < msg.length(); offset += 64) {
        uint32_t w[64];
        
        // Prepare message schedule
        for (int i = 0; i < 16; i++) {
            w[i] = ((uint32_t)(unsigned char)msg[offset + i * 4 + 0] << 24) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 1] << 16) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 2] << 8) |
                   ((uint32_t)(unsigned char)msg[offset + i * 4 + 3]);
        }
        
        for (int i = 16; i < 64; i++) {
            w[i] = sha256_sigma1(w[i-2]) + w[i-7] + sha256_sigma0(w[i-15]) + w[i-16];
        }
        
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], hh = h[7];
        
        for (int i = 0; i < 64; i++) {
            uint32_t T1 = hh + sha256_Sigma1(e) + sha256_ch(e, f, g) + sha256_k[i] + w[i];
            uint32_t T2 = sha256_Sigma0(a) + sha256_maj(a, b, c);
            hh = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }
        
        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }
    
    // Produce final hash (big-endian)
    stringstream ss;
    ss << hex << setfill('0');
    for (int i = 0; i < 8; i++) {
        ss << setw(8) << h[i];
    }
    
    output = ss.str();
}

void hashas(const string &ivestis, string &isvestis) 
{
    string konvertuotasIvestis = convertLithuanianText(ivestis);
    // Atkomentuokite, kurio norite:
    // md5Hash(konvertuotasIvestis, isvestis);
    // sha1Hash(konvertuotasIvestis, isvestis);
    sha256Hash(konvertuotasIvestis, isvestis);  
}

