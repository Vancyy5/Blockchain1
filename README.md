# MANO HASH'AS

Ši funkcija generuoja unikalų maišos (hash) kodą tekstams, kurie gali turėti lietuviškų simbolių. Ši versija yra v0.11, kurioje patobulinau savo sukurtą originalų hash generatorių, nežiūrint kaip veikia patikimi ir geri generatoriai.

---
# VEIKIMO APRAŠYMAS
---
## __Pseudo-kodu:__ 
```cpp 
function hashas(ivestis, isvestis) 
{
    isvestis.clear()
    konvertuotasIvestis = convertLithuanianText(ivestis)

    seedString = ""
    seedui = ""
    
    if konvertuotasIvestis is not empty
    {
        // Kas 1000 simbolių ASCII suma
        for kiekviena 1000 simbolių grupė in konvertuotasIvestis
            suma = 0
            for kiekvienas simbolis grupėje (iki 10 simbolių)
                suma += ASCII vertė simbolio
            seedString += suma kaip string
        
        // Kas 20 simbolių '1' bitų kiekis
        for kiekviena 20 simbolių grupė in konvertuotasIvestis
            ones = 0
            for kiekvienas simbolis grupėje
                ones += '1' bitų skaičius simbolyje
            seedString += ones kaip string
            seedui += ones kaip string
    }
    else
        seedString = "0"

    binaryInput = binary representation of konvertuotasIvestis
    
    if binaryInput is empty
        binaryInput = "10000000"
    
    // Prailginimas iki 256 bitų su XOR
    originalBinary = binaryInput
    while binaryInput.size() < 256
        toAdd = originalBinary
        for kiekvienas bitas in toAdd (kol nepasiekiame 256)
            newBit = binaryInput[i % binaryInput.size()] XOR toAdd[i]
            binaryInput += newBit
    
    if binaryInput.size() > 256
        binaryInput = first 256 bits

    mySeed = safeStringToUint32(seedString, seedui)
    rng = mt19937(mySeed)

    mixedBinary = ""
    for kiekviena 32 bitų bloką in binaryInput
        randVal = rng()
        for kiekvienas bitas bloką
            bit ^= atitinkamas randVal bitas
            mixedBinary += bit

    isvestis = convert mixedBinary to HEX (po 4 bitus)
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

## 3. Sėklos generavimas (seed)

Hash funkcija generuoja atsitiktinių bitų generatoriaus sėklą pagal įvestį. Aš dariau taip iš įvesties:

- Susumuoja ASCII kodus kas 1000 simbolių.  
- Suskaičiuoja „1“ bitų kiekį kas 20 simbolių.  
- Rezultatai sudedami į seedString
- Suskaičiuoti bitų kiekiai sudedami į seedui
- Gautas skaičius naudojamas kaip sėkla atsitiktinių skaičių generatoriui.
- Jeigu seed'as didesnis negu 9 simboliai, imami pirmi 9 simboliai.

- Kiekvienas seedui simbolis pridedamas prie pradinio seed naudojant formulę:
```cpp 
seed = seed * 31 + simbolio_kodas
```
- Taip gaunamas pradinės sėklos skaičius.
- Galutinio hash’o skaičiavimas
- Į hash priskiriamas seed.
- Per kiekvieną truncated simbolį atliekama operacija:
```cpp 
hash = hash * seed + simbolio_kodas
```
- Tai sujungia abiejų eilučių informaciją.

---

## 4. Binarinė reprezentacija

Įvestis paverčiama į 256 bitų binarinę seką string `binaryInput`. Jei reikia, seka prailginama(kartojama) arba apkarpoma iki 256 bitų. Jei tuščia, pridedamas bent 1 baitas (10000000).

---

## 5. XOR maišymas

1. Imamas 32 bitų segmentas iš `binaryInput` sekos.
2. Sugeneruojamas 32 bitų atsitiktinis skaičius iš RNG su suskaičiuotu seed'u.
3. Kiekvienas įvesties bitas XOR’inamas su atitinkamu atsitiktiniu bitu:
   - Jei bitai skirtingi → rezultatas 1
   - Jei bitai vienodi → rezultatas 0
4. Rezultatas įrašomas į `mixedBinary` seką.
5. Jei likę mažiau nei 32 bitai, imami tiek, kiek yra.

---

## 6. Konversija į HEX

Maišyta binarinė seka konvertuojama į šešioliktainę (HEX) eilutę. Kiekvieni 4 binariniai bitai paverčiami į vieną HEX simbolį. Galutinis rezultatas yra unikalus hash kodas.

---
# MANO HASH'O TESTAVIMAS
---
## 1. Išvedimo dydys

Patikrinamas išvedimo dydis – nepriklausomai nuo įvedimo, rezultatas visada tokio pat ilgio.

Panaudojus šiuos failus gaunami hash'ai:

1000 simbolių stringo failo large_1000.txt :
5c2a864017f0d63becbc32d44b08edd9bee741cdc3d3ac563bb9affa00316e70

vieno simbolio failo single_a.txt  :
6a9cf67dafcb0ac3b71126cc79622852c11cc9a450ab1aa8255d001e4e3c5673

tuščio failo empty.txt  :
807c5e20cc5074b68aa47c2723aeec22da9537120ddad98d02ed913263c3cdd8

__Rezultatas__ : visada būna to pačio ilgio su visais failais (64 simbolių hex formatu).


---
## 2. Deterministiškumas

Patikrink deterministiškumą – tas pats failas duoda tą patį hash’ą,

Paėmus failą single_a.txt visada išlieka tas pats hash'as :  6a9cf67dafcb0ac3b71126cc79622852c11cc9a450ab1aa8255d001e4e3c5673

__Rezultatas__: mano hash'as yra deterministinis. 

---
## 3. Efektyvumas

Išbandytas konstitucija.txt failas su 1, 2, 4, 8, 16, 32, 64 ir 128 eilutėmis.

Žemiau pateikti grafikai su gautais vidurkiais 5 bandymų.

![Nuotrauka](<nuotraukos/Screenshot 2025-10-01 153334.png>)

![alt text](<nuotraukos/Screenshot 2025-10-01 153655.png>)

__Rezultatas__: Tik kai pasiekia 128 eilučių, paryškėja užtruktas laikas.

---
## 4. Kolizijų paieška

Naudojamas failas collision_pairs.txt, kuriame yra po 100 000 atsitiktinių string porų, kurių ilgis yra: 10, 100, 500, 1000 simbolių.

Žemiau pateikta nuotrauka su gautais rezultatais.

![alt text](<nuotraukos/Screenshot 2025-10-01 152558.png>)

__Rezultatas__: nerandamos jokios kolizijos iš visų 400 000 atsistiktinių string porų. 

---
## 5. Lavinos efektas

Naudojamas failas avalanche_test_pairs.txt, kuriame yra 100 000 string porų, kurių ilgiai yra 10, 50, 100, 500 ir skiriasi atsitiktiniu vienu simboliu tarpusavyje.

Žemiau pateikta nuotrauka, kurioje suskaičiuota, kiek procentų skiriasi gautieji porų hash'ai:
1. bit'ų lygmeniu,
2. hex’ų lygmeniu.
ir parodytos minimalios, maksimalios ir vidutines skirtingumo reikšmės.

![Nuotrauka](<nuotraukos/Screenshot 2025-10-01 154330.png>)

__Rezultatas__: 
Bitų skirtumas ir hexų skirtumas 50, 100 ir 500 ilgio porose yra beveik vienodi (apie 42% bitų, apie 78% hexų).

Lavinos efektas išlieka panašus 50, 100 ir 500 ilgio porose.

---
## 5.  Negrįžtamumo demonstracija

Žemiau parodytos nuotraukos, kaip hash'as dirba su HASH(imput + salt).

![Nuotrauka](<nuotraukos/Screenshot 2025-10-01 152736.png>)

Ir taip pat kaip reaguoja ieškomas hashas, prasidedantis iš tų pačių simbolių po 100 000 bandymų.

![alt text](<nuotraukos/Screenshot 2025-10-01 152803.png>)

__Rezultatas__: 
Negalima žinant gautą hash'ą ir saltą atpažinti įvesties.

Hash'as atsparus, kai programa bando atspėti (bruteforce) originalų tekstą pagal jo hash reikšmę.

---
# IŠVADOS:
---
## Stiprybės:
Pastovi išvestis - visada 256 bitų

Deterministinė - patikimai veikia

Aukštas kolizijų atsparumas 

Lietuviškų simbolių palaikymas 

Geras Lavinos efektas. Bitų skirtumai svyruoja apie 41–50%, o hex skirtumai ~78–93%.

---

## Trūkumai:

Lavinos efekte vis tiek pasitaiko kolizijų 

128 eilučių failas užtruko pastebimai ilgiau (neefektingas).


---
# PALYGINIMAS SU V0.1:
---
V0.11 yra žymiai geresnė Lavinos efekto srityje, bet truputį letesnė apdorajant konstitucija.txt failo eilutes.