#include "funkcijos.h"

// Visi galimi simboliai
vector<char> allChars;

void hashas (const string &ivestis, string &isvestis)
{
    for (char c : ivestis)
    {
        int ascii = static_cast<int>(c);  
        cout << ascii << " ";
        
        isvestis += to_string(ascii);
    }
}

void Chars() 
{
    allChars.clear();
    
    for (int i = 32; i <= 126; i++) {
        allChars.push_back(static_cast<char>(i));
    }
    
}

void initAllChars() 
{
    allChars.clear();
    
    for (int i = 32; i <= 126; i++) {
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
    
    uniform_int_distribution<size_t> asciiDist(0, allChars.size() - 1);
    uniform_int_distribution<size_t> ltDist(0, lithuanianChars.size() - 1);
    uniform_int_distribution<int> typeDist(0, 9); 
    
    string result;
    result.reserve(length * 2); // Rezervuojame daugiau vietos UTF-8 simboliams
    
    for (size_t i = 0; i < length; i++) {
        if (typeDist(rng) < 8) { 
            result += allChars[asciiDist(rng)];
        } else { 
            result += lithuanianChars[ltDist(rng)];
        }
    }
    
    return result;
}

void createSingleCharFiles() 
{
    // Sukuriame aplanką jei jo nėra
    system("mkdir -p failai");
    
    vector<string> testChars = {"a", "b", "ą"}; 
    vector<string> fileNames = {"single_a.txt", "single_b.txt", "single_lt.txt"};
    
    for (size_t i = 0; i < testChars.size(); i++) {
        string filename = "failai/" + fileNames[i];
        ofstream file(filename);
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
     system("mkdir -p failai");
    
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
        
        ofstream file(filename);
        if (file.is_open()) 
        {
            file << content;
            file.close();
        }
    }
}

void createSimilarFiles() 
{
    cout << "Kuriami failai, kurie skiriasi tik viduriniu simboliu..." << endl;
    
    system("mkdir -p failai");
    
    random_device rd;
    mt19937 rng(rd());
    
    const size_t fileSize = 2000;
    
    // Generuojame bazinį stringą
    string baseString = generateRandomString(fileSize, rng);
    

    size_t middlePos = fileSize / 2;
    
    // 1. Failas su lietuviška raide viduryje
    string modified1 = baseString;
    modified1[middlePos] = 'a'; 
    
    ofstream file1("failai/similar_middle_lt.txt");
    if (file1.is_open()) {
        file1 << modified1;
        file1.close();
    }
    
    string modified2 = baseString;
    modified2[middlePos] = '9';
    
    ofstream file2("failai/similar_middle_num.txt");
    if (file2.is_open()) {
        file2 << modified2;
        file2.close();
    }

    string modified3 = baseString;
    modified3[middlePos] = '#';
    
    ofstream file3("failai/similar_middle_special.txt");
    if (file3.is_open()) {
        file3 << modified3;
        file3.close();;
    }
}