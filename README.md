# Mano hash'as

Ši funkcija generuoja unikalų maišos (hash) kodą tekstams, kurie gali turėti lietuviškų simbolių. 

---

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
5. Jei likę mažiau nei 32 bitai, imamas tiek, kiek yra.

---

## 6. Konversija į HEX

Maišyta binarinė seka konvertuojama į šešioliktainę (HEX) eilutę. Kiekvieni 4 binariniai bitai paverčiami į vieną HEX simbolį. Galutinis rezultatas yra unikalus hash kodas.