#include "funkcijos.h"

vector<char> allChars;

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


void hashas(const string &ivestis, string &isvestis) {
    isvestis.clear();

    string konvertuotasIvestis = convertLithuanianText(ivestis);

    string seedString;

    // kas 10 simbolių ASCII suma
    for (size_t i = 0; i < konvertuotasIvestis.size(); i += 10) {
        int suma = 0;
        for (size_t j = i; j < i + 10 && j < konvertuotasIvestis.size(); j++) {
            suma += static_cast<unsigned char>(konvertuotasIvestis[j]);
        }
        seedString += to_string(suma);
    }

    // kas 20 simbolių '1' bitų kiekis
    for (size_t i = 0; i < konvertuotasIvestis.size(); i += 20) {
        int ones = 0;
        for (size_t j = i; j < i + 20 && j < konvertuotasIvestis.size(); j++) {
            bitset<8> bits(static_cast<unsigned char>(konvertuotasIvestis[j]));
            ones += bits.count();
        }
        seedString += to_string(ones);
    }

    // Įvesties binary kodas
    string binaryInput;
    for (unsigned char c : konvertuotasIvestis) {
        binaryInput += bitset<8>(c).to_string();
    }

    // prailginam iki 256 bitų
    while (binaryInput.size() < 256) binaryInput += binaryInput;
    if (binaryInput.size() > 256) binaryInput = binaryInput.substr(0, 256);

   
    uint32_t mySeed = stoul(seedString);
    mt19937 rng(mySeed);

    // Maišymas
    string mixedBinary;
    for (size_t i = 0; i < binaryInput.size(); i += 32) {
        uint32_t randVal = rng();
        for (size_t j = 0; j < 32 && i + j < binaryInput.size(); j++) 
        {
            int bit = binaryInput[i + j] - '0';
            int rbit = (randVal >> j) & 1;
            bit ^= rbit;
            mixedBinary.push_back(bit ? '1' : '0');
        }
    }

    // konversija į HEX
    for (size_t i = 0; i + 4 <= mixedBinary.size(); i += 4) 
    {
        string nibble = mixedBinary.substr(i, 4);
        int value = stoi(nibble, nullptr, 2);
        stringstream ss;
        ss << hex << value;
        isvestis += ss.str();
    }
}

void initAllChars() 
{
    allChars.clear();
      
    for (int i = 33; i <= 126; i++) {
        allChars.push_back(static_cast<char>(i));
    }
}

string generateRandomString(size_t length, mt19937 &rng) 
{
    if (allChars.empty()) {
        initAllChars();
    }
    

    vector<string> lithuanianChars = {"ą", "č", "ę", "ė", "į", "š", "ų", "ū", "ž", 
                                      "Ą", "Č", "Ę", "Ė", "Į", "Š", "Ų", "Ū", "Ž"};
    
    vector<string> allPossibleChars;

    for (char c : allChars) 
    {
        allPossibleChars.push_back(string(1, c));
    }
    
    for (const string& ltChar : lithuanianChars) 
    {
        allPossibleChars.push_back(ltChar);
    }
    
    uniform_int_distribution<size_t> charDist(0, allPossibleChars.size() - 1);
    
    string result;
    result.reserve(length * 3); // Daugiau vietos UTF-8 simboliams
    
    for (size_t i = 0; i < length; i++) {
        result += allPossibleChars[charDist(rng)];
    }
    
    return result;
}

void createSingleCharFiles() 
{
    #ifdef _WIN32
        system("if not exist failai mkdir failai >nul 2>&1");
    #else
        system("mkdir -p failai >/dev/null 2>&1");
    #endif
    
    vector<string> testChars = {"a", "b", "ą"}; 
    vector<string> fileNames = {"single_a.txt", "single_b.txt", "single_lt.txt"};
    
    for (size_t i = 0; i < testChars.size(); i++) {
        string filename = "failai/" + fileNames[i];
        ofstream file(filename, ios::out | ios::binary); // Binary režimas UTF-8 simboliams
        if (file.is_open()) {
            file << testChars[i];
            file.close();
        }
    }
    
    ofstream emptyFile("failai/empty.txt");
    emptyFile.close();
}

void createLargeRandomFiles() 
{
    #ifdef _WIN32
        system("if not exist failai mkdir failai >nul 2>&1");
    #else
        system("mkdir -p failai >/dev/null 2>&1");
    #endif
    
    random_device rd;
    mt19937 rng(rd());
    
    vector<pair<string, size_t>> fileSizes = 
    {
        {"large_1000.txt", 1000},
        {"large_5000.txt", 5000},
        {"large_10000.txt", 10000}
    };
    
    for (const auto& fileInfo : fileSizes) {
        string filename = "failai/" + fileInfo.first;
        string content = generateRandomString(fileInfo.second, rng);
        
        ofstream file(filename, ios::out | ios::binary); // Binary režimas UTF-8 simboliams
        if (file.is_open()) 
        {
            file << content;
            file.close();
        }
    }
}

void createSimilarFiles() 
{
    #ifdef _WIN32
        system("if not exist failai mkdir failai >nul 2>&1");
    #else
        system("mkdir -p failai >/dev/null 2>&1");
    #endif
    
    random_device rd;
    mt19937 rng(rd());
    
    const size_t fileSize = 2000;
    
    string baseString = generateRandomString(fileSize, rng);
    

    size_t middlePos = fileSize / 2;
    
    // Jei pataikėme į UTF-8 simbolio vidurį, grįžtame prie simbolio pradžios
    while (middlePos > 0 && 
           (static_cast<unsigned char>(baseString[middlePos]) & 0xC0) == 0x80) {
        middlePos--;
    }
 
    string modified1 = baseString;
    if (middlePos < modified1.length()) {
    
        size_t endPos = middlePos;
        while (endPos + 1 < modified1.length() && 
               (static_cast<unsigned char>(modified1[endPos + 1]) & 0xC0) == 0x80) {
            endPos++;
        }
        modified1.erase(middlePos, endPos - middlePos + 1);
        modified1.insert(middlePos, "ą");
    }
    
    ofstream file1("failai/similar_middle_lt.txt", ios::out | ios::binary);
    if (file1.is_open()) {
        file1 << modified1;
        file1.close();
    }
    

    string modified2 = baseString;
    if (middlePos < modified2.length()) {
     
        size_t endPos = middlePos;
        while (endPos + 1 < modified2.length() && 
               (static_cast<unsigned char>(modified2[endPos + 1]) & 0xC0) == 0x80) {
            endPos++;
        }
        modified2.erase(middlePos, endPos - middlePos + 1);
        modified2.insert(middlePos, "2");
    }
    
    ofstream file2("failai/similar_middle_num.txt", ios::out | ios::binary);
    if (file2.is_open()) {
        file2 << modified2;
        file2.close();
    }

    string modified3 = baseString;
    if (middlePos < modified3.length()) {
       
        size_t endPos = middlePos;
        while (endPos + 1 < modified3.length() && 
               (static_cast<unsigned char>(modified3[endPos + 1]) & 0xC0) == 0x80) {
            endPos++;
        }
        modified3.erase(middlePos, endPos - middlePos + 1);
        modified3.insert(middlePos, "#");
    }
    
    ofstream file3("failai/similar_middle_special.txt", ios::out | ios::binary);
    if (file3.is_open()) {
        file3 << modified3;
        file3.close();
    }
}

void generateCollisionTestPairs() 
{
    #ifdef _WIN32
        system("if not exist failai mkdir failai >nul 2>&1");
    #else
        system("mkdir -p failai >/dev/null 2>&1");
    #endif
    
    random_device rd;
    mt19937 rng(rd());
    
    vector<size_t> lengths = {10, 100, 500, 1000};
    const size_t pairsPerLength = 25000; // 25k porų kiekvienam ilgiui = 100k iš viso
    
    ofstream outFile("failai/collision_pairs.txt", ios::out | ios::binary);
    if (!outFile.is_open()) {
        cerr << "Nepavyko sukurti collision_pairs.txt failo!" << endl;
        return;
    }
    
    for (size_t length : lengths) {
        
        for (size_t i = 0; i < pairsPerLength; i++) {
        
            string str1 = generateRandomString(length, rng);
            string str2 = generateRandomString(length, rng);
            
            outFile << str1 << " " << str2 << "\n";
            
        }
    }
    
    outFile.close();
}

// NAUJA FUNKCIJA: Lavinos efekto testui
void generateAvalancheTestPairs() 
{
    #ifdef _WIN32
        system("if not exist failai mkdir failai >nul 2>&1");
    #else
        system("mkdir -p failai >/dev/null 2>&1");
    #endif
    
    random_device rd;
    mt19937 rng(rd());
    
    vector<size_t> lengths = {10, 50, 100, 500}; // Skirtingi ilgiai lavinos testui
    const size_t pairsPerLength = 25000; // 25k porų kiekvienam ilgiui = 100k iš viso
    
    ofstream outFile("failai/avalanche_test_pairs.txt", ios::out | ios::binary);
    if (!outFile.is_open()) {
        cerr << "Nepavyko sukurti avalanche_test_pairs.txt failo!" << endl;
        return;
    }
    
    if (allChars.empty()) {
        initAllChars();
    }
    
    for (size_t length : lengths) {
        
        for (size_t i = 0; i < pairsPerLength; i++) {
        
            string str1 = generateRandomString(length, rng);
     
            string str2 = str1;
            
            // Randame atsitiktinę poziciją viduryje
            uniform_int_distribution<size_t> posDist(length/4, 3*length/4);
            size_t changePos = posDist(rng);
            
            // Užtikriname, kad pozicija yra ASCII simbolio pradžioje (ne UTF-8 viduryje)
            while (changePos < str2.length() && 
                   (static_cast<unsigned char>(str2[changePos]) & 0xC0) == 0x80) {
                changePos = (changePos > 0) ? changePos - 1 : changePos + 1;
                if (changePos >= str2.length()) changePos = length / 2;
            }
            
            // Pakeičiame simbolį į kitą ASCII simbolį
            uniform_int_distribution<size_t> charDist(0, allChars.size() - 1);
            char newChar;
            do {
                newChar = allChars[charDist(rng)];
            } while (newChar == str2[changePos]); // Užtikriname, kad simbolis tikrai keičiasi
            
            str2[changePos] = newChar;
            
            outFile << str1 << " " << str2 << "\n";
            
        }
        outFile << "\n";
    }
    
    outFile.close();

}