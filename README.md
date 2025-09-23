# MANO HASH'AS

Ši funkcija generuoja unikalų maišos (hash) kodą tekstams, kurie gali turėti lietuviškų simbolių. Ši versija yra v0.1, kurioje sukūriau savo originalų hash generatorių, nežiūrint kaip veikia patikimi ir geri generatoriai.

---
# VEIKIMO APRAŠYMAS
---
## __Pseudo-kodu:__ 
```cpp 
function hashas(ivestis, isvestis)
{
    isvestis.clear()
    konvertuotasIvestis = convertLithuanianText(ivestis)

    if konvertuotasIvestis is not empty
    {
        seedString = ""
        for kiekviena 10 simbolių grupė in konvertuotasIvestis
            seedString += ASCII suma grupėje
        for kiekviena 20 simbolių grupė in konvertuotasIvestis
            seedString += '1' bitų skaičius grupėje
    }
    else
        seedString = "0"

    binaryInput = binary representation of konvertuotasIvestis
    if binaryInput is empty
        binaryInput = "10000000"
    pad binaryInput to 256 bits

    mySeed = safeStringToUint32(seedString)
    rng = mt19937(mySeed)

    mixedBinary = ""
    for kiekviena 32 bitų bloką in binaryInput
        randVal = rng()
        for kiekvienas bitas bloką
            bit ^= randVal bit
            mixedBinary += bit

    isvestis = convert mixedBinary to HEX
}

function safeStringToUint32(str)
{
    if str is empty
        return 12345
    truncate str to last 9 characters if too long
    try convert str to uint32
    catch error
        hash = 0
        for kiekvienas simbolis c in str
            hash = hash * 31 + c
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

- Susumuoja ASCII kodus kas 10 simbolių.  
- Suskaičiuoja „1“ bitų kiekį kas 20 simbolių.  
- Gautas skaičius naudojamas kaip sėkla atsitiktinių skaičių generatoriui.
- Jeigu seed'as didesnis negu 9 simboliai, imami paskutiniai 9 simboliai.

---

## 4. Binarinė reprezentacija

Įvestis paverčiama į 256 bitų binarinę seką string `binaryInput`. Jei reikia, seka prailginama(kartojama) arba apkarpoma iki 256 bitų.

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
4682a22eb69d4e6414289966b2d2e79c12fbe908d15a2a0f5e9fe7d90ffe71f9

vieno simbolio failo single_a.txt  :
7ccb4dd6fc3a1e80b452c0976c108a6b746f23efb3ad657eaffaee2b5362a081

tuščio failo empty.txt  :
b5d07eb174d5a3692ee5e86d83b3049b424df2d95fcac55b43ee3e51663e279b

__Rezultatas__ : visada būna to pačio ilgio su visais failais (64 simbolių hex formatu).


---
## 2. Deterministiškumas

Patikrink deterministiškumą – tas pats failas duoda tą patį hash’ą,

Paėmus failą single_a.txt visada išlieka tas pats hash'as :  7ccb4dd6fc3a1e80b452c0976c108a6b746f23efb3ad657eaffaee2b5362a081

__Rezultatas__: mano hash'as yra deterministinis. 

---
## 3. Efektyvumas

Išbandytas konstitucija.txt failas su 1, 2, 4, 8, 16, 32, 64 ir 128 eilutėmis.

Žemiau pateikti grafikai su gautais vidurkiais 5 bandymų.

![Nuotrauka](<nuotraukos/Screenshot 2025-09-23 183824.png>)

![alt text](<nuotraukos/Screenshot 2025-09-23 191732.png>)

__Rezultatas__: Tik kai pasiekia 128 eilučių, paryškėja užtruktas laikas.

---
## 4. Kolizijų paieška

Naudojamas failas collision_pairs.txt, kuriame yra po 100 000 atsitiktinių string porų, kurių ilgis yra: 10, 100, 500, 1000 simbolių.

Žemiau pateikta nuotrauka su gautais rezultatais.

![alt text](<nuotraukos/Screenshot 2025-09-23 192104.png>)

__Rezultatas__: nerandamos jokios kolizijos iš visų 400 000 atsistiktinių string porų. 

---
## 5. Lavinos efektas

Naudojamas failas avalanche_test_pairs.txt, kuriame yra 100 000 string porų, kurių ilgiai yra 10, 50, 100, 500 ir skiriasi atsitiktiniu vienu simboliu tarpusavyje.

Žemiau pateikta nuotrauka, kurioje suskaičiuota, kiek procentų skiriasi gautieji porų hash'ai:
1. bit'ų lygmeniu,
2. hex’ų lygmeniu.
ir parodytos minimalios, maksimalios ir vidutines skirtingumo reikšmės.

![Nuotrauka](<nuotraukos/Screenshot 2025-09-23 183902.png>)

__Rezultatas__: 
Lavinos efektas yra stipresnis trumpesniuose duomenų blokuose.

Max bitų pokytis išlieka panašus nepaisant ilgio.


---
## 5.  Negrįžtamumo demonstracija

Žemiau parodytos nuotraukos, kaip hash'as dirba su HASH(imput + salt).

![Nuotrauka](<nuotraukos/Screenshot 2025-09-23 183902.png>)

Ir taip pat kaip reaguoja ieškomas hashas, prasidedantis iš tų pačių simbolių po 100000 bandymų.

![alt text](<nuotraukos/Screenshot 2025-09-23 183929.png>)

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

---

## Trūkumai:

Lavinos efektas nepakankamai stiprus ir nepastovus. Vidutinis skirtumas 45-47%. 

Seed generavimas nuspėjamas.

128 eilučių failas užtruko pastebimai ilgiau (neefektingas).
