#include "funkcijos.h"
#include "failugeneravimas.h"
#include "laikas.h"

int main(int argc, char* argv[]) 
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); 
    SetConsoleCP(CP_UTF8);
#endif

    string ivestis;
    string isvestis;

    cout << "Ar norite duomenis ivesti ranka?(t/n)" << endl;
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
        hashas(ivestis, isvestis);
        cout << "Rezultatas: " << isvestis << endl;
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

        cout << "Skaitomas failas: " << filepath << endl;
    
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
        
        hashas(fileContent, isvestis);
   
        cout << "Rezultatas: " << isvestis << endl;
       }
    }

    return 0;
}
    