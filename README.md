# STANDARTINIAI HASH

Ši versija yra v0.2palyginimas, kurioje panaudoju standartinius hash'us MD5, SHA-1, SHA-256 palyginimui su savo sukurtomis hash'o versijomis v0.1, v0.11 ir v0.2.  

---
# HASH'Ų LYGINIMAI
---
## 1. Efektyvumas

Išbandytas konstitucija.txt failas su 1, 2, 4, 8, 16, 32, 64 ir 128 eilutėmis.

Žemiau pateikta lentelė su gautais duomenimis su kiekvienu algoritmu paėmus vidurkį iš 5 bandymų

| Algoritmas| ILGIS (1) | ILGIS (2) |ILGIS (4) |ILGIS (8) | ILGIS (16) | ILGIS (32) | ILGIS (64) | ILGIS (128) |
|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|
| V0.1|0.000095s| 0.000070s |0.000103s| 0.000108s | 0.000227s |  0.000486s | 0.000778s |0.002142s |
| V0.11|0.000194s| 0.000189s |0.000227s| 0.000340s | 0.000705s| 0.000855 |  0.000986s | 0.004040s | 
| V0.2|0.000269s| 0.000238s |0.000269s| 0.000298s |  0.000446s |  0.000688s | 0.001434s |0.002686s |
| MD5|0.000053s| 0.000049s |0.000041s| 0.000101s | 0.000220s |  0.000241s | 0.000709s |0.001232s |
| SHA-1| 0.000288s| 0.000130s |0.000111s | 0.000167s | 0.000244s |  0.000416s | 0.000792s |0.001775s |
| SHA-256| 0.000060s| 0.000157s |0.000085s | 0.000173s |  0.000287s |  0.000634s | 0.001193s |0.002690s |

__Rezultatas__: 

- Visuose algoritmuose matomas laiko augimas didėjant failo ilgiui.

- Iki ~32–64 eilučių skirtumai nėra labai ryškūs. Tik prie 128 eilučių akivaizdžiai paryškėja užtruktas laikas.

- Greičiausi algoritmai mažiems failams: MD5, V0.1, SHA-256. Lėčiausi – V0.11 ir V0.2.

---
## 2. Kolizijų paieška

Naudojamas failas collision_pairs.txt, kuriame yra po 100 000 atsitiktinių string porų, kurių ilgis yra: 10, 100, 500, 1000 simbolių.

Žemiau pateikta lentelė su gautais duomenimis kiekvieno algoritmo

| Algoritmas| ILGIS (10) | ILGIS (100) |ILGIS (500) |ILGIS (1000) |
|-------------------------|-------------------------|-------------------------|-------------------------|
| V0.1            | 0 kolizijų            | 0 kolizijų           |0 kolizijų  | 0 kolizijų  |
| V0.11            | 0 kolizijų            | 0 kolizijų           |0 kolizijų  | 0 kolizijų  |
| V0.2            | 0 kolizijų            | 0 kolizijų           |0 kolizijų  | 0 kolizijų  |
| MD5           | 0 kolizijų            | 0 kolizijų           |0 kolizijų  | 0 kolizijų  |
| SHA-1           | 0 kolizijų            | 0 kolizijų           |0 kolizijų  | 0 kolizijų  |
| SHA-256          | 0 kolizijų            | 0 kolizijų           |0 kolizijų  | 0 kolizijų  |

__Rezultatas__: Nei viename algoritme kolizijų neaptikta, net didesniems stringams.

---
## 3. Lavinos efektas

Naudojamas failas avalanche_test_pairs.txt, kuriame yra 100 000 string porų, kurių ilgiai yra 10, 50, 100, 500 ir skiriasi atsitiktiniu vienu simboliu tarpusavyje.

Žemiau pateikta nuotrauka, kurioje suskaičiuota, kiek procentų skiriasi gautieji porų hash'ai:
1. bit'ų lygmeniu,
2. hex’ų lygmeniu.

 Lentelėje surašomi kiekvieno algoritmo rezultatai su 500 ilgio stringo porom.

| Algoritmas| Hex lygmuo (vidurkis) | Bit lygmuo (vidurkis) |
|-------------------------|-------------------------|-------------------------|
| V0.1  | 26,01%         | 13,85%         |
| V0.11    |  78,24%         | 41,69%         |
| V0.2             |  93,75%         | 49,97%         |
| MD5            | 93,76%         | 50,03%         |
| SHA-1           |  93,75%         | 50,03%         |
| SHA-256          |  93.74%         | 50.01%         |

__Rezultatas__: 
- Bitų ir hex’ų skirtumai tarp panašių stringų siekia ~50% ir ~93% – Lavinos efektas veikia efektyviai.

- V0.2, MD5, SHA-1, SHA-256 parodo optimalų lavinos efektą.

- V0.1 silpniau reaguoja į minimalų įvesties pakeitimą.

---
## 4.  Negrįžtamumo demonstracija

Žemiau parodytos nuotraukos, kaip hash'as dirba su HASH(imput + salt).

Lyginimui naudojau Žinutė: "Slapta žinutė" su salt: "salt123".

Lentelėje parodyta, kaip visi algoritmai užhashino.

| Algoritmas| HASH|
|-------------------------|-------------------------|
| V0.1  |  e0270a1f456a7e72009209eaf83829a968d2675e4496d5b084731b9fcc4c4252     |
| V0.11    |    50f61203318ab90c0171710469494fd75c57fd77e413501199ade8790ab891ed       |
| V0.2             |   874dd59867950c6c3e42a448c5e8a2c22051bf41eb6aa11217b858ee5b2700f1     |
| MD5            | 40120409984f74fcc9f076026d1305ec |
| SHA-1           |  fee620d72a3b8704b90d7625fe70d5d9a5822eff  |
| SHA-256          |   55fbd3ccc44f2cd2a3f7fc72ce523ba828ff760199dcf49d089913fdca0f5ec5         |

Ir taip pat kaip reaguoja ieškomas hashas, prasidedantis iš tų pačių simbolių po 100 000 bandymų.

Naudojamas hash'as, kuris prasideda '0000'

Lentelėje parodoma, ar surastas toks hash'as ir kiek laiko užtruko ir bandymų. Jeigu nesurastas lentelėje yra '-'.

| Algoritmas| Bandymai | Laikas (s) |
|-------------------------|-------------------------|-------------------------|
| V0.1  | -           | -         |
| V0.11    | -           | -         |
| V0.2             | 82652            | 6.3s          |
| MD5            | 16967          | 0.1544s         |
| SHA-1           | 2887         | 0.0259s         |
| SHA-256          | 59656           | 0.5560s          |

__Rezultatas__: 
- Negalima žinant gautą hash'ą ir saltą atpažinti įvesties visuose algoritmuose.

- Bruteforce atskleidžia, kad MD5 ir SHA-1 yra palyginti greiti atspėjimui mažų prefiksų (0000) atveju, V0.2 ir SHA-256 – lėtesni.

- Tai parodo, kad algoritmai yra saugūs, bet ne visi vienodai atsparūs bruteforce

---
# IŠVADOS:

## 1. Greitis:

Mažiems failams greičiausi: MD5, V0.1, SHA-256.

Didėjant failo dydžiui, skirtumas tarp algoritmų ryškėja, V0.11 yra lėčiausias.

## 2. Kolizijos:

Nei vienas algoritmas neparodė kolizijų 100 000 porų testuose, t.y., visi algoritmai yra kolizijų atžvilgiu patikimi.

## 3. Lavinos efektas:

V0.2, MD5, SHA-1 ir SHA-256 demonstruoja stiprų lavinos efektą (~50% bitų, ~93% hex’ų), užtikrinantį gerą atsitiktinumo pasiskirstymą.

V0.1 lavinos efektas silpnesnis.

## 4. Negrįžtamumas:

Visi algoritmai užtikrina, kad originalios žinutės neįmanoma atkurti iš hash'o ir salto.

Bruteforce bandymai rodo, kad paprasti prefiksai gali būti atspėti, bet tai priklauso nuo algoritmo greičio.

## Rekomendacijos:

Jei svarbus greitis: naudoti MD5 arba V0.1 (tik mažesniems failams).

Jei svarbiausia saugumas ir lavinos efektas: rekomenduojami V0.2, SHA-1 ar SHA-256.

