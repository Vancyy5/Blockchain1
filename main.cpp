#include "funkcijos.h"
#include "failugeneravimas.h"
#include "laikas.h"
#include "testavimas.h"
#include "visi.h"

// Globalus pasirinktas hash tipas
enum HashType {
    HASH_MIGLE = 1,
    HASH_JULIAUS = 2,
    HASH_TEJOS = 3,
    HASH_GUSTAVO = 4,
    HASH_NEDOS = 5,
    HASH_MANO = 6,
    HASH_VISI = 7
};

HashType selectedHash = HASH_MANO; // Numatytasis

// Wrapper funkcija, kuri kviečia pasirinktą hash
void hashWrapper(const string& input, string& output) {
    switch(selectedHash) {
        case HASH_MIGLE:
            output = Migle::hash(input);
            break;
        case HASH_JULIAUS:
            output = Juliaus::hash(input);
            break;
        case HASH_TEJOS:
            output = Tejos::hash(input);
            break;
        case HASH_GUSTAVO:
            output = Gustavo::hash(input);
            break;
        case HASH_NEDOS:
            output = Nedos::hash(input);
            break;
        case HASH_MANO:
            output = Mano::hash(input);
            break;
        case HASH_VISI:
            output = Visi::hash(input);
            break;
        default:
            hashas(input, output); // Originalus
            break;
    }
}

string getHashName(HashType type) {
    switch(type) {
        case HASH_MIGLE: return "Miglės Hash";
        case HASH_JULIAUS: return "Juliaus Hash";
        case HASH_TEJOS: return "Tėjos Hash";
        case HASH_GUSTAVO: return "Gustavo Hash";
        case HASH_NEDOS: return "Nedos Hash";
        case HASH_MANO: return "Mano Hash";
        case HASH_VISI: return "Kombinuotas Hash";
        default: return "Nežinomas Hash";
    }
}

void selectHashAlgorithm() {
    cout << "\n=== HASH ALGORITMO PASIRINKIMAS ===" << endl;
    cout << "1 - Miglės Hash " << endl;
    cout << "2 - Juliaus Hash " << endl;
    cout << "3 - Tėjos Hash " << endl;
    cout << "4 - Gustavo Hash " << endl;
    cout << "5 - Nedos Hash " << endl;
    cout << "6 - Mano Hash " << endl;
    cout << "7 - Kombinuotas Hash " << endl;
    cout << "\nPasirinkimas (1-7): ";
    
    int choice;
    cin >> choice;
    
    if (choice >= 1 && choice <= 7) {
        selectedHash = static_cast<HashType>(choice);
        cout << "Pasirinktas: " << getHashName(selectedHash) << endl;
    } else {
        cout << "Netinkamas pasirinkimas! Naudojamas numatytasis (Jūsų Hash)." << endl;
        selectedHash = HASH_MANO;
    }
}

int main(int argc, char* argv[]) 
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); 
    SetConsoleCP(CP_UTF8);
    #endif

    string ivestis;
    string isvestis;

    // Pasirinkti hash algoritmą
    selectHashAlgorithm();

    cout << "\nAr norite atlikti testavimus? (t/n): ";
    char testChoice;
    cin >> testChoice;

    if (testChoice == 't' || testChoice == 'T') {
        cout << "\nPasirinkite testavimo tipą:" << endl;
        cout << "1 - Efektyvumo testavimas (su konstitucija.txt)" << endl;
        cout << "2 - Kolizijų paieška" << endl;
        cout << "3 - Lavinos efekto testavimas" << endl;
        cout << "4 - Negrįžtamumo demonstracija" << endl;
        cout << "5 - Visi testai" << endl;
        cout << "Pasirinkimas: ";
        
        char choice;
        cin >> choice;
        
        // Testuose naudojame wrapper funkciją
        cout << "\nTestavimas naudojant: " << getHashName(selectedHash) << endl;
        
        switch(choice) {
            case '1':
                testEfficiency();
                break;
            case '2':
                testCollisions();
                break;
            case '3':
                testAvalancheEffect();
                break;
            case '4':
                testIrreversibility();
                break;
            case '5':
                testEfficiency();
                testCollisions();
                testAvalancheEffect();
                testIrreversibility();
                break;
            default:
                cout << "Netinkamas pasirinkimas!" << endl;
                break;
        }
        
        cout << "\nAr norite tęsti su įprastomis funkcijomis? (t/n): ";
        cin >> testChoice;
        while (testChoice != 't' && testChoice != 'T' && testChoice != 'n' && testChoice != 'N') 
        {
            cout << "Netinkama ivestis. Bandykite dar karta: ";
            cin >> testChoice;
        }
        if (testChoice == 'n' || testChoice == 'N') 
        {
            return 0;
        }
    }

    cout << "\nAr norite duomenis ivesti ranka?(t/n)" << endl;
    char ats;
    cin >> ats;

    while (ats != 't' && ats != 'T' && ats != 'n' && ats != 'N') 
    {
        cout << "Netinkama ivestis. Bandykite dar karta: ";
        cin >> ats;
    }

    if(ats == 'T' ||  ats == 't')
    {
        cout << "Iveskite teksta: ";
        cin.ignore(); 
        
        getline(cin, ivestis);
        hashWrapper(ivestis, isvestis);
        cout << "Rezultatas (" << getHashName(selectedHash) << "): " << isvestis << endl;
    }
    
    else if(ats == 'N' || ats == 'n')
    {
        cout<<"Ar norite generuoti nauja faila?(t/n)" << endl;
        cin >> ats;

        while (ats != 't' && ats != 'T' && ats != 'n' && ats != 'N') 
        {
            cout << "Netinkama ivestis. Bandykite dar karta: ";
            cin >> ats;
        }

        if(ats == 'T' ||  ats == 't')
        {
            initAllChars();
          
            char testChoice;
            cout << "Koki faila norite sugeneruoti:\n";
            cout << "1 - vieno simbolio failai ir tuscia\n";
            cout << "2 - failai su daug simboliu\n";
            cout << "3 - failai su daug simboliu, kurie skiriasi viduriniu simboliu\n";
            cout << "4 - failai koliziju testavimui\n";
            cout << "5 - failai lavinos efekto testavimui\n";
            cout << "Kitas - programos pabaiga\n";
            cout << "Pasirinkimas: ";

            cin >> testChoice;
          
            if (testChoice == '1') 
            {
                createSingleCharFiles();
            }
            else if (testChoice == '2') 
            {
                createLargeRandomFiles();
            }
            else if (testChoice == '3') 
            {
                createSimilarFiles();
            }
            else if (testChoice == '4') 
            {
                generateCollisionTestPairs(); 
            }
            else if (testChoice == '5') 
            {
                generateAvalancheTestPairs();
            }
            else 
            {
                cout << "Programos pabaiga" << endl;
                return 0;
            }
        }
        else if(ats == 'N' || ats == 'n')
        {
            string filename;

            if(argc >= 2) {
                filename = argv[1];
            } else {
                cout << "Parasykite failo pavadinima: ";
                cin >> filename;
            }

            string filepath = "failai/" + filename;

            ifstream file(filepath);
            if(!file.is_open()) 
            {
                cerr << "Klaida: Nepavyko atidaryti failo: " << filename << endl;
                return 1;
            }
    
            string line;
            string fileContent;
            while(getline(file, line)) 
            {
                fileContent += line + "\n";
            }
            file.close();
        
            if (!fileContent.empty() && fileContent.back() == '\n')
            {
                fileContent.pop_back();
            }
        
            Laikas hashTimer("Hash'avimas");
            hashTimer.pradeti();
            hashWrapper(fileContent, isvestis);
            hashTimer.baigti();
        
            cout << "Hash rezultatas (" << getHashName(selectedHash) << "): " << isvestis << endl;
   
            // Sukuriame failaipo katalogą jei neegzistuoja
            #ifdef _WIN32
                system("if not exist failaipo mkdir failaipo >nul 2>&1");
            #else
                system("mkdir -p failaipo >/dev/null 2>&1");
            #endif
        
            string outpath = "failaipo/" + filename;
            ofstream outfile(outpath);
            if (!outfile.is_open()) 
            {
                cerr << "Klaida: nepavyko sukurti failo: " << outpath << endl;
                return 1;
            }

            outfile << isvestis << endl;
            outfile.close();
        }
    }

    return 0;
}