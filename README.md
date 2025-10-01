# MANO HASH'AS

Ši funkcija generuoja unikalų maišos (hash) kodą tekstams, kurie gali turėti lietuviškų simbolių. Ši versija yra v0.2, kurioje patobulinau savo sukurtą originalų hash generatorių su dirbtinio intelekto pagalbą.

---
# VEIKIMO APRAŠYMAS
---
## __Pseudo-kodu:__ 
```cpp 
function hashas(ivestis, isvestis) 
{
    isvestis.clear()
    konvertuotasIvestis = convertLithuanianText(ivestis)

    // Konstantos avalanche efektui
    PRIME1 = 0x9E3779B185EBCA87
    PRIME2 = 0xC2B2AE3D27D4EB4F
    PRIME3 = 0x165667B19E3779F9
    PRIME4 = 0x85EBCA77C2B2AE63
    
    // 512-bitų būsenos inicializacija
    state[8] = {0, 0, 0, 0, 0, 0, 0, 0}
    seed = PRIME1
    
    if konvertuotasIvestis is not empty
    {
        // Įvesties ilgio įtaka
        seed ^= konvertuotasIvestis.size() * PRIME2
        
        // ASCII sumos su bit rotacija
        for kiekvienas simbolis in konvertuotasIvestis
            seed = rotateLeft(seed, 7) XOR (simbolis * PRIME3)
        
        // Pozicijos-priklausomas mixing
        for kiekvienas simbolis in konvertuotasIvestis
            stateIdx = pozicija % 8
            state[stateIdx] ^= simbolis * (PRIME4 + pozicija)
            state[stateIdx] = rotateLeft(state[stateIdx], 13)
    }
    else
        seed = PRIME1
        state[0] = PRIME2

    // Avalanche mixing seed'ui
    seed = avalancheMix(seed, PRIME2, PRIME3)
    
    rng = mt19937_64(seed)

    // 4 mixing raundai
    for round = 0 to 3
        // State mixing su RNG
        for i = 0 to 7
            state[i] ^= rng()
            state[i] = avalancheMix(state[i], PRIME2, PRIME3)
        
        // State'ų tarpusavio maišymas
        for i = 0 to 7
            next = (i + 1) % 8
            prev = (i + 7) % 8
            state[i] ^= rotateLeft(state[next], 17) XOR rotateLeft(state[prev], 31)

    // Binary reprezentacija
    binaryInput = binary representation of konvertuotasIvestis
    
    if binaryInput is empty
        binaryInput = "10000000"
    
    // Prailginimas iki 512 bitų su 3-way XOR
    originalBinary = binaryInput
    targetSize = 512
    
    while binaryInput.size() < targetSize
        for kiekvienas bitas in originalBinary (kol nepasiekiame 512)
            pos1 = i % binaryInput.size()
            pos2 = (i * 7) % originalBinary.size()
            pos3 = (i * 13) % binaryInput.size()
            
            bit1 = binaryInput[pos1]
            bit2 = originalBinary[pos2]
            bit3 = binaryInput[pos3]
            
            // Majority function
            sum = bit1 + bit2 + bit3
            newBit = (sum >= 2) ? '1' : '0'
            binaryInput += newBit
    
    binaryInput = first 512 bits

    // Finalus mixing su 3-way XOR
    finalBinary = ""
    for kiekviena 64 bitų bloką in binaryInput
        randVal = rng()
        stateVal = state[blokoNumeris % 8]
        
        for kiekvienas bitas bloką
            inputBit = bitas iš binaryInput
            randBit = atitinkamas randVal bitas
            stateBit = atitinkamas stateVal bitas
            
            finalBit = inputBit XOR randBit XOR stateBit
            finalBinary += finalBit

    // Permutacija (Fisher-Yates shuffle)
    permutation = [0, 1, 2, ..., 511]
    shuffleRng = mt19937(seed XOR PRIME4)
    
    for i = 511 down to 1
        j = shuffleRng() % (i + 1)
        swap(permutation[i], permutation[j])
    
    scrambledBinary = ""
    for i = 0 to 511
        scrambledBinary += finalBinary[permutation[i]]

    // Konversija į HEX (pirmi 256 bitai)
    isvestis = convert first 256 bits of scrambledBinary to HEX (po 4 bitus)
}

function avalancheMix(h, PRIME2, PRIME3) {
    h ^= h >> 33
    h *= PRIME2
    h ^= h >> 29
    h *= PRIME3
    h ^= h >> 32
    return h
}

function safeStringToUint32(str, seedui) {
    truncated = str
    if truncated.length > 9
        truncated = first 9 characters of str
    
    seed = 0
    for kiekvienas simbolis in seedui
        seed = seed * 31 + ASCII vertė simbolio
    
    hash = seed
    
    for kiekvienas simbolis in truncated
        hash = hash * seed + ASCII vertė simbolio
    
    return hash
}
```
## __Žodžiais:__

## 1. Lietuviškų simbolių žemėlapis

Funkcija priskiria lietuviškiems simboliams 16-bitų kodus. Tai leidžia konvertuoti simbolius į nuoseklią baitų seką prieš maišos generavimą.

---

## 2. Teksto konvertavimas

Įvestas tekstas yra konvertuojamas taip, kad įprasti simboliai lieka UTF-8 formatu, o lietuviški simboliai paverčiami į 16-bitų baitus pagal žemėlapį. Taip gaunama nuosekli baitų seka, paruošta maišai.

---

## 3. Kriptografinės konstantos ir būsenos inicializacija

Hash funkcija naudoja 4 didelius pirminius skaičius (PRIME1-PRIME4), kurie užtikrina gerą bitų pasiskirstymą:

- PRIME1: Pradinis seed'as
- PRIME2, PRIME3: Avalanche mixing'ui
- PRIME4: Papildomam maišymui

Sukuriama 512-bitų vidinė būsena (8 × 64-bitų skaičiai), kuri saugo tarpinę informaciją viso maišymo proceso metu

---
## 4. Sėklos generavimas (seed)

Seed'as generuojamas iš įvesties charakteristikų:

1. Ilgio įtaka: Įvesties ilgis dauginamas su PRIME2 ir XOR'inamas su seed'u.
2. ASCII sumos su rotacija: Kiekvienas simbolis:

- Dauginamas su PRIME3
- XOR'inamas su seed'u
- Seed'as pasukamas 7 bitais kairėn (bit rotation)


3. Pozicijos-priklausomas mixing: Kiekvienas simbolis:

- Dauginamas su (PRIME4 + pozicija)
- XOR'inamas į atitinkamą state poziciją (pozicija % 8)
- State elementas pasukamas 13 bitų kairėn

---

## 5. Avalanche efektas

Avalanche mixing užtikrina, kad net mažas įvesties pokytis dramatiškai pakeičia išvestį:
```cpp 
cppseed ^= seed >> 33    // Sklaido aukštus bitus
seed *= PRIME2        // Maišo bitus tarpusavyje
seed ^= seed >> 29    // Dar kartą sklaido
seed *= PRIME3        // Galutinis maišymas
seed ^= seed >> 32    // Finalizuoja
```
Šis procesas pritaikomas ir seed'ui, ir visiems state elementams.

---

## 6. State maišymo raundai

Atliekami 4 maišymo raundai, kiekviename:

1. RNG mixing: Kiekvienas state elementas:

- XOR'inamas su 64-bitų atsitiktiniu skaičiumi iš RNG
- Praeinamas per avalanche mixing


2. State'ų tarpusavio maišymas: Kiekvienas state elementas:

- XOR'inamas su kaimyniniais elementais
- Kaimynai pasukti skirtingais kampais (17 ir 31 bitas)
- Tai užtikrina informacijos sklaidą per visą state'ą

---

## 7. Binarinė reprezentacija ir prailginimas

Įvestis paverčiama į binarinę seką. Jei tuščia, pridedamas bent 1 baitas (10000000).
Prailginimas iki 512 bitų naudojant 3-way XOR su majority function:

- Imami 3 bitai iš skirtingų pozicijų (naudojant skirtingus offset'us: 1, 7, 13)
- Jei bent 2 iš 3 bitų yra '1' → naujas bitas '1'
- Priešingu atveju → '0'

Tai sukuria sudėtingesnį pattern'ą nei paprastas XOR.

---

## 8. Finalus maišymas (3-way XOR)

Kiekvienam bitui atliekamas 3-way XOR:

1. Bitas iš prailgintos įvesties
2. Bitas iš RNG generuoto skaičiaus
3. Bitas iš atitinkamo state elemento

```cpp
cppfinalBit = inputBit XOR randBit XOR stateBit
```

Tai sujungia visus tris informacijos šaltinius į vieną išvestį.

---

## 9. Permutacija (Fisher-Yates shuffle)

Bitai permaišomi naudojant Fisher-Yates shuffle algoritmą:

- Sukuriamas permutacijų masyvas [0, 1, 2, ..., 511]
- Su deterministiniu RNG (seed XOR PRIME4) masyvas sumaišomas
- Galutinė binarinė seka gaunama perrašant bitus pagal permutaciją

Tai prideda papildomą difuziją ir apsunkina pattern'ų atpažinimą.

---

## 10. Konversija į HEX

Pirmi 256 bitai iš scrambled sekos konvertuojami į šešioliktainę (HEX) eilutę. Kiekvieni 4 binariniai bitai paverčiami į vieną HEX simbolį.

Galutinis rezultatas: 64 HEX simboliai (256 bitų hash'as).

---
# HASH'O TESTAVIMAS
---
## 1. Išvedimo dydys

Patikrinamas išvedimo dydis – nepriklausomai nuo įvedimo, rezultatas visada tokio pat ilgio.

Panaudojus šiuos failus gaunami hash'ai:

1000 simbolių stringo failo large_1000.txt :
cac03660e5c87882526702995179e353e7a9a48bbf26923020fc716f1255116b

vieno simbolio failo single_a.txt  :
31a8c571ec101ff4023a9a56d6897a56311557cfe114833063d9597824651d71

tuščio failo empty.txt  :
7757ea720541cf7f47f71f725ff68c28a8cab5130bd8709f1df7981d471d808d

__Rezultatas__ : visada būna to pačio ilgio su visais failais (64 simbolių hex formatu).


---
## 2. Deterministiškumas

Patikrink deterministiškumą – tas pats failas duoda tą patį hash’ą,

Paėmus failą single_a.txt visada išlieka tas pats hash'as :  31a8c571ec101ff4023a9a56d6897a56311557cfe114833063d9597824651d71

__Rezultatas__: mano hash'as yra deterministinis. 

---
## 3. Efektyvumas

Išbandytas konstitucija.txt failas su 1, 2, 4, 8, 16, 32, 64 ir 128 eilutėmis.

Žemiau pateikti grafikai su gautais vidurkiais 5 bandymų.

![Nuotrauka](<nuotraukos/Screenshot 2025-10-01 163334.png>)

![alt text](<nuotraukos/Screenshot 2025-10-01 163925.png>)

__Rezultatas__: Tik kai pasiekia 128 eilučių, paryškėja užtruktas laikas.

---
## 4. Kolizijų paieška

Naudojamas failas collision_pairs.txt, kuriame yra po 100 000 atsitiktinių string porų, kurių ilgis yra: 10, 100, 500, 1000 simbolių.

Žemiau pateikta nuotrauka su gautais rezultatais.

![alt text](<nuotraukos/Screenshot 2025-10-01 163527.png>)

__Rezultatas__: nerandamos jokios kolizijos iš visų 400 000 atsistiktinių string porų. 

---
## 5. Lavinos efektas

Naudojamas failas avalanche_test_pairs.txt, kuriame yra 100 000 string porų, kurių ilgiai yra 10, 50, 100, 500 ir skiriasi atsitiktiniu vienu simboliu tarpusavyje.

Žemiau pateikta nuotrauka, kurioje suskaičiuota, kiek procentų skiriasi gautieji porų hash'ai:
1. bit'ų lygmeniu,
2. hex’ų lygmeniu.
ir parodytos minimalios, maksimalios ir vidutines skirtingumo reikšmės.

![Nuotrauka](<nuotraukos/Screenshot 2025-10-01 163624.png>)

__Rezultatas__: 
Bitų skirtumas ir hexų skirtumas 10, 50, 100 ir 500 ilgio porose yra beveik vienodi (apie 50% bitų, apie 93% hexų).

Lavinos efektas išlieka efetyvus visokiuose string'ų ilgių pororse ir užtvirtina, kad nėra kolizijų

---
## 5.  Negrįžtamumo demonstracija

Žemiau parodytos nuotraukos, kaip hash'as dirba su HASH(imput + salt).

![Nuotrauka](<nuotraukos/Screenshot 2025-10-01 163632.png>)

Ir taip pat kaip reaguoja ieškomas hashas, prasidedantis iš tų pačių simbolių po 100 000 bandymų.

![alt text](<nuotraukos/Screenshot 2025-10-01 163650.png>)

__Rezultatas__: 
Negalima žinant gautą hash'ą ir saltą atpažinti įvesties.

Hash'as neatsparus, kai programa bando atspėti (bruteforce) originalų tekstą pagal jo hash reikšmę.

---
# IŠVADOS:
---
## Stiprybės:
Pastovi išvestis - visada 256 bitų

Deterministinė - patikimai veikia

Aukštas kolizijų atsparumas 

Lietuviškų simbolių palaikymas 

Tobulas Lavinos efektas. Bitų skirtumai svyruoja apie 50%, o hex skirtumai apie 93%. 

Nera kolizijų lavinos testavime.

---

## Trūkumai:

Hash'as neatsparus bruteforc'ui.


---
# PALYGINIMAS SU V0.11 ir V0.1:
---
V0.2 yra žymiai geresnė Lavinos efekto srityje, užtikrinant, kad nėra kolizijų.

Hash'as nebelieka atsparus bruteforc'ui, kaip V0.11, ar V0.1.

Laikas užtruktas kontitucijos.txt failo eilučių hash'inimui yra panašus kaip V0.1 su didesnio skaičiaus eilutėmis, bet V0.1 yra pastebimai greitesnis su mažiau skaičių eilučių.

## Pagrindiniai skirtumai nuo senesnės versijos:
- 512-bitų vidinė būsena vietoj tiesioginio seed'o generavimo
- Avalanche efektas užtikrina geresnį bitų pasiskirstymą
- 4 mixing raundai su state maišymu
- 3-way XOR vietoj 2-way
- Majority function prailginime
- Fisher-Yates permutacija papildomam scrambling'ui
- 64-bitų RNG (mt19937_64) vietoj 32-bitų