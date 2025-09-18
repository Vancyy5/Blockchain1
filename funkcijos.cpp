#include "funkcijos.h"

vector<char> allChars;

void hashas (const string &ivestis, string &isvestis)
{
    for (char c : ivestis)
    {
        int ascii = static_cast<int>(c);   
        if(isvestis.length() <64)       //nu negerai nes kur reikiami simboliai?????
        {
           isvestis += to_string(ascii); //idek ir lietuviskas
        }
    }

    while(isvestis.length() <64)
        {
            isvestis +=  isvestis;
        }

    //nuo 64 iki 127

    //negalima sumazint nes gali prarast reikalinga simboli 
    //mums reikia min 128 simboliu iki 192
    string isvestisnaujas1;
    string isvestisnaujas2;
    
    for (int i=0; i<isvestis.length(); i+=2)
     {
        if ( i%2 == 0 ) isvestisnaujas1+=isvestis[i];
        if ( i%2 == 1 ) isvestisnaujas1+=isvestis[i];
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
    result.reserve(length * 2); // Rezervuojame daugiau vietos UTF-8 simboliams
    
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
        
        ofstream file(filename);
        if (file.is_open()) 
        {
            file << content;
            file.close();
        }
    }
}

void createSimilarFiles() //tikrai kazkur klaida su lietuviskom raidem
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
    
    while (middlePos < baseString.length() && 
           (static_cast<unsigned char>(baseString[middlePos]) > 127)) {
        middlePos++; 
    }
    if (middlePos >= baseString.length()) middlePos = fileSize / 2;
    
    baseString[middlePos] = 'X';
    
    string modified1 = baseString;
    modified1.erase(middlePos, 1);
    modified1.insert(middlePos, "ą");
    
    ofstream file1("failai/similar_middle_lt.txt");
    if (file1.is_open()) {
        file1 << modified1;
        file1.close();
    }
    
    string modified2 = baseString;
    modified2[middlePos] = '2';
    
    ofstream file2("failai/similar_middle_num.txt");
    if (file2.is_open()) {
        file2 << modified2;                         //sutvarkyk idk?
        file2.close();
    }

    string modified3 = baseString;
    modified3[middlePos] = '#';
    
    ofstream file3("failai/similar_middle_special.txt");
    if (file3.is_open()) {
        file3 << modified3;
        file3.close();
    }
}