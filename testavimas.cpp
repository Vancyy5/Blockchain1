#include "testavimas.h"

// Efektyvumo testavimas
void testEfficiency() {
    
    string constitution = readConstitutionFile();
    if (constitution.empty()) {
        cout << "Nepavyko nuskaityti konstitucijos failo!" << endl;
        return;
    }
    
    vector<string> lines = splitIntoLines(constitution);
    cout << "Konstitucijos faile rasta " << lines.size() << " eilučių" << endl;
    
    vector<int> lineCounts = {1, 2, 4, 8, 16, 32, 64, 128};
    vector<double> avgTimes;
    
    cout << "\nMatuojamas hash'avimo laikas su skirtingais duomenų kiekiais:" << endl;
    cout << setw(15) << "Eilučių sk." << setw(15) << "Vidutinis laikas (s)" << endl;
    cout << string(30, '-') << endl;
    
    for (int lineCount : lineCounts) 
    {
        if (lineCount > static_cast<int>(lines.size())) break;
        
        vector<double> times;
        measureHashingTime(constitution, lineCount, times);
        
        double avgTime = accumulate(times.begin(), times.end(), 0.0) / times.size();
        avgTimes.push_back(avgTime);
        
        cout << setw(15) << lineCount << setw(15) << fixed << setprecision(6) << avgTime << endl;
    }
    
    createEfficiencyGraph(lineCounts, avgTimes);
}

void measureHashingTime(const string& content, int lineMultiplier, vector<double>& times) 
{
    vector<string> lines = splitIntoLines(content);
    string testContent;
    
    // Sukuriame testinį turinį su nurodytu eilučių skaičiumi
    for (int i = 0; i < lineMultiplier && i < static_cast<int>(lines.size()); i++) 
    {
        testContent += lines[i] + "\n";
    }
    
    const int iterations = 5; // Pakartojimų skaičius
    
    for (int i = 0; i < iterations; i++) {
        Laikas timer("Hash testavimas " + to_string(i + 1));
        timer.pradeti();
        
        string hash_result;
        hashas(testContent, hash_result);
        
        timer.baigti();
        times.push_back(timer.gautiLaikoSkirtuma());
    }
}

void createEfficiencyGraph(const vector<int>& lineCounts, const vector<double>& avgTimes) {
  
    // Rasti maksimalų laiką skalės nustatymui
    double maxTime = *max_element(avgTimes.begin(), avgTimes.end());
    
    for (size_t i = 0; i < lineCounts.size() && i < avgTimes.size(); i++) {
        cout << setw(8) << lineCounts[i] << " |";
        
        int barLength = static_cast<int>((avgTimes[i] / maxTime) * 50);
        for (int j = 0; j < barLength; j++) {
            cout << "█";
        }
        cout << " " << fixed << setprecision(6) << avgTimes[i] << "s" << endl;
    }
}

// Kolizijų paieška
void testCollisions() {
    cout << "\n=== KOLIZIJŲ TESTAVIMAS ===" << endl;
    
    ifstream collisionFile("failai/collision_pairs.txt");
    if (!collisionFile.is_open()) {
        cout << "Nepavyko atidaryti collision_pairs.txt failo!" << endl;
        cout << "Pirmiau sugeneruokite kolizijų testavimo failus." << endl;
        return;
    }

    vector<int> lengths = {10, 100, 500, 1000};
    const int pairsPerLength = 25000;
    
    cout << setw(15) << "Ilgis" << setw(15) << "Kolizijos" << setw(15) << "Tikimybė %" << endl;
    cout << string(45, '-') << endl;

    string line;
    
    for (int length : lengths) {
        int collisions = 0;
        int totalPairs = 0;
        
        for (int i = 0; i < pairsPerLength; i++) {
            if (!getline(collisionFile, line) || line.empty()) {
                cout << "KLAIDA: Nepavyko nuskaityti pakankamai duomenų ilgiui " << length << endl;
                break;
            }
            
            stringstream ss(line);
            string str1, str2;
            ss >> str1 >> str2;
            
            if (str1.empty() || str2.empty()) {
                cout << "KLAIDA: Tuščias stringas eilutėje: " << line << endl;
                continue;
            }
            
            string hash1, hash2;
            hashas(str1, hash1);
            hashas(str2, hash2);
            
            // Tikriname kolizijas tarp str1 ir str2
            if (hash1 == hash2) {
                collisions++;
                cout << "KOLIZIJA RASTA! Ilgis: " << length << endl;
            }
            
            totalPairs++;
        }
        double probability = 0.0;
        if (totalPairs > 0) {
            probability = (static_cast<double>(collisions) / totalPairs) * 100;
        }
        
        cout << setw(15) << length << setw(15) << collisions 
             << setw(14) << fixed << setprecision(4) << probability << "%" << endl;
    }
    
    collisionFile.close();
}

// Lavinos efekto testavimas 
void testAvalancheEffect() {
    cout << "\n=== LAVINOS EFEKTO TESTAVIMAS ===" << endl;

    ifstream avalancheFile("failai/avalanche_test_pairs.txt");
    if (!avalancheFile.is_open()) {
        cout << "Nepavyko atidaryti avalanche_test_pairs.txt failo!" << endl;
        cout << "Pirmiau sugeneruokite lavinos efekto testavimo failus." << endl;
        return;
    }

    vector<int> lengths = {10, 50, 100, 500};
    const int pairsPerLength = 25000;

    cout << setw(8) << "Ilgis" 
         << setw(12) << "Bit skirt." 
         << setw(12) << "Hex skirt." 
         << setw(10) << "Min bit" 
         << setw(10) << "Max bit" 
         << setw(10) << "Min hex" 
         << setw(10) << "Max hex" << endl;
    cout << string(72, '-') << endl;

    string line;
    
    for (int length : lengths) {
        vector<double> bitDifferences;
        vector<double> hexDifferences;
        bitDifferences.reserve(pairsPerLength);
        hexDifferences.reserve(pairsPerLength);

        for (int i = 0; i < pairsPerLength; i++) {
            if (!getline(avalancheFile, line) || line.empty()) {
                cout << "KLAIDA: Nepavyko nuskaityti pakankamai duomenų ilgiui " << length << endl;
                break;
            }
            
            stringstream ss(line);
            string str1, str2;
            ss >> str1 >> str2;
            
            if (str1.empty() || str2.empty()) {
                cout << "KLAIDA: Tuščias stringas eilutėje: " << line << endl;
                continue;
            }
            
            string hash1, hash2;
            hashas(str1, hash1);
            hashas(str2, hash2);

            double bitDiff = calculateBitDifference(hash1, hash2);
            double hexDiff = calculateHexDifference(hash1, hash2);

            bitDifferences.push_back(bitDiff);
            hexDifferences.push_back(hexDiff);
        }

        if (bitDifferences.empty()) {
            cout << "KLAIDA: Nėra duomenų ilgiui " << length << endl;
            continue;
        }

        double avgBitDiff = accumulate(bitDifferences.begin(), bitDifferences.end(), 0.0) / bitDifferences.size();
        double avgHexDiff = accumulate(hexDifferences.begin(), hexDifferences.end(), 0.0) / hexDifferences.size();

        double minBit = *min_element(bitDifferences.begin(), bitDifferences.end());
        double maxBit = *max_element(bitDifferences.begin(), bitDifferences.end());
        double minHex = *min_element(hexDifferences.begin(), hexDifferences.end());
        double maxHex = *max_element(hexDifferences.begin(), hexDifferences.end());

        cout << setw(8) << length
             << setw(11) << fixed << setprecision(2) << avgBitDiff << "%"
             << setw(11) << fixed << setprecision(2) << avgHexDiff << "%"
             << setw(9) << fixed << setprecision(1) << minBit << "%"
             << setw(9) << fixed << setprecision(1) << maxBit << "%"
             << setw(9) << fixed << setprecision(1) << minHex << "%"
             << setw(9) << fixed << setprecision(1) << maxHex << "%" << endl;
    }

    avalancheFile.close();
}

double calculateBitDifference(const string& hash1, const string& hash2) {
    if (hash1.length() != hash2.length()) {
        cout << "KLAIDA: Hash'ai skirtingo ilgio! " << hash1.length() << " vs " << hash2.length() << endl;
        return 0.0;
    }
    
    int differentBits = 0;
    int totalBits = 0;
    
    for (size_t i = 0; i < hash1.length(); i++) {
       
        int val1, val2;
        
        if (hash1[i] >= '0' && hash1[i] <= '9') {
            val1 = hash1[i] - '0';
        } else if (hash1[i] >= 'a' && hash1[i] <= 'f') {
            val1 = hash1[i] - 'a' + 10;
        }
         else {
            cout << "KLAIDA: Netinkamas hex simbolis: " << hash1[i] << endl;
            continue;
        }
        
        if (hash2[i] >= '0' && hash2[i] <= '9') {
            val2 = hash2[i] - '0';
        } else if (hash2[i] >= 'a' && hash2[i] <= 'f') {
            val2 = hash2[i] - 'a' + 10;
        } else {
            cout << "KLAIDA: Netinkamas hex simbolis: " << hash2[i] << endl;
            continue;
        }
        
        // XOR ir skaičiuojame skirtingus bitus
        int xorResult = val1 ^ val2;
        for (int bit = 0; bit < 4; bit++) {
            if ((xorResult >> bit) & 1) {
                differentBits++;
            }
            totalBits++;
        }
    }
    
    if (totalBits == 0) return 0.0;
    return (static_cast<double>(differentBits) / totalBits) * 100.0;
}

double calculateHexDifference(const string& hash1, const string& hash2) {
    if (hash1.length() != hash2.length()) return 0.0;
    
    int differentHexChars = 0;
    for (size_t i = 0; i < hash1.length(); i++) {
        if (hash1[i] != hash2[i]) {
            differentHexChars++;
        }
    }
    return (static_cast<double>(differentHexChars) / hash1.length()) * 100.0;
}

// Negrįžtamumo demonstracija 
void testIrreversibility() {
    cout << "\n=== NEGRĮŽTAMUMO DEMONSTRACIJA ===" << endl;
    
    // Hiding property demonstracija
    cout << "\n--- HIDING PROPERTY ---" << endl;
    demonstrateHiding("Slapta žinutė", "salt123");
    demonstrateHiding("Kita slapta žinutė", "salt123");
    demonstrateHiding("Slapta žinutė", "kitasalt");
    
    cout << "\n--- PUZZLE FRIENDLINESS ---" << endl;
    demonstratePuzzleFriendliness();
}

string addSalt(const string& input, const string& salt) {
    return input + salt;
}

void demonstrateHiding(const string& message, const string& salt) {
    string saltedMessage = addSalt(message, salt);
    string hash_result;
    hashas(saltedMessage, hash_result);
    
    cout << "Žinutė: \"" << message << "\"" << endl;
    cout << "Su salt: \"" << salt << "\"" << endl;
    cout << "Hash: " << hash_result << endl;
    cout << endl;
}

// puzzle friendliness funkcija
void demonstratePuzzleFriendliness() {
    
    vector<string> targets = {"0000", "aaaa", "1111"}; 
    
    for (const string& target : targets) {
        cout << "\nIeškoma hash'o, kuris prasideda '" << target << "'..." << endl;
        
        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> dist(0, 999999);
        
        string baseMessage = "Blockchain puzzle: ";
        int attempts = 0;
        int maxAttempts = 100000; 
        bool found = false;
        
        Laikas timer("Puzzle sprendimas");
        timer.pradeti();
        
        for (attempts = 1; attempts <= maxAttempts; attempts++) {
            string nonce = to_string(dist(rng));
            string message = baseMessage + nonce;
            
            string hash_result;
            hashas(message, hash_result);
            
            // Tikriname ar hash pradžia atitinka target
            if (hash_result.length() >= target.length() && 
                hash_result.substr(0, target.length()) == target) {
                timer.baigti();
                cout << "RASTA! Nonce: " << nonce << endl;
                cout << "Žinutė: \"" << message << "\"" << endl;
                cout << "Hash: " << hash_result << endl;
                cout << "Bandymų skaičius: " << attempts << endl;
                cout << "Laikas: " << fixed << setprecision(4) << timer.gautiLaikoSkirtuma() << "s" << endl;
                found = true;
                break;
            }
            
            if (attempts % 10000 == 0) {
                cout << "Bandymų: " << attempts << "... (paskutinis hash: " 
                     << hash_result.substr(0, 8) << "...)" << endl;
            }
        }
        
        if (!found) {
            timer.baigti();
            cout << "Nepavyko rasti per " << maxAttempts << " bandymų." << endl;
        }
        
    }
}

// Pagalbinės funkcijos
string readConstitutionFile() {
    ifstream file("failai/konstitucija.txt");
    if (!file.is_open()) {
        cout << "Nepavyko atidaryti konstitucijos failo!" << endl;
        cout << "Įdėkite konstitucija.txt failą į 'failai' katalogą." << endl;
        return "";
    }
    
    string content, line;
    while (getline(file, line)) {
        content += line + "\n";
    }
    
    file.close();
    return content;
}

vector<string> splitIntoLines(const string& content) {
    vector<string> lines;
    stringstream ss(content);
    string line;
    
    while (getline(ss, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

void printStatistics(const vector<double>& values, const string& description) {
    if (values.empty()) return;
    
    double sum = accumulate(values.begin(), values.end(), 0.0);
    double mean = sum / values.size();
    double min_val = *min_element(values.begin(), values.end());
    double max_val = *max_element(values.begin(), values.end());
    
    cout << description << ":" << endl;
    cout << "  Vidurkis: " << fixed << setprecision(2) << mean << endl;
    cout << "  Minimumas: " << fixed << setprecision(2) << min_val << endl;
    cout << "  Maksimumas: " << fixed << setprecision(2) << max_val << endl;
}