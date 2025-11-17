# Grupinė integracija

Ši versija yra grupineintegracija, kurioje apjungiau 6 studentų sukurtus hashus, pagal jų naujausia, be dirbtinio intelekto darytas funkcija. Taip pat atlikau lyginamąją analizę.

Naudojau:

1. Miglės - Bubble sort algoritmas su hash skaičiavimu per swap'us 
2. Juliaus Vilkanec- XOR ir shift operacijos su 8 seed'ų masyvu + finalizacijos raundai 
3. Tėjos - Prime skaičiais pagrįstos operacijos su bit rotacijomis 
4. Nedos - Three-in-one mixer ir value-dependent shuffle 
5. Savo - Random number generator maišymas su bitų skaičiavimu 
6. Gustavo - Binary permutacijos ir swap algoritmas 

# HASHO TESTAVIMAS

## 1. Išvedimo dydys
Patikrinamas išvedimo dydis – nepriklausomai nuo įvedimo, rezultatas visada tokio pat ilgio.

Panaudojus šiuos failus gaunami hash'ai:

1000 simbolių stringo failo large_1000.txt : fb16c0c73523b0e73561c07cf6ec8ea635e7861efeb16c60cb246c197b453e9a

vieno simbolio failo single_a.txt : af21412bd031a456a215f24129a5780c29b9c33c0d0bbf1a772bc51689e3b3a8

tuščio failo empty.txt : 7d7b4064f34eef68c8e062d02ddf9699c33230905a53476123692ee9ea8d2aa9

Rezultatas : visada būna to pačio ilgio su visais failais (64 simbolių hex formatu).

---
## 2. Deterministiškumas
Patikrink deterministiškumą – tas pats failas duoda tą patį hash’ą,

Paėmus failą single_a.txt visada išlieka tas pats hash'as : af21412bd031a456a215f24129a5780c29b9c33c0d0bbf1a772bc51689e3b3a8

Rezultatas: hash'as yra deterministinis.

---
## 3. Efektyvumas
Išbandytas konstitucija.txt failas su 1, 2, 4, 8, 16, 32, 64 ir 128 eilutėmis.

Žemiau pateiktas grafikas su gautais vidurkiais 5 bandymų.

![alt text](<nuotraukos/Screenshot 2025-10-08 225746.png>)

Rezultatas: Hash'as yra greitas.

---
## 4. Kolizijų paieška
Naudojamas failas collision_pairs.txt, kuriame yra po 100 000 atsitiktinių string porų, kurių ilgis yra: 10, 100, 500, 1000 simbolių.

Žemiau pateikta nuotrauka su gautais rezultatais.

![alt text](<nuotraukos/Screenshot 2025-10-08 231200.png>)

Rezultatas: nerandamos jokios kolizijos iš visų 400 000 atsistiktinių string porų.

---
## 5. Lavinos efektas
Naudojamas failas avalanche_test_pairs.txt, kuriame yra 100 000 string porų, kurių ilgiai yra 10, 50, 100, 500 ir skiriasi atsitiktiniu vienu simboliu tarpusavyje.

Žemiau pateikta nuotrauka, kurioje suskaičiuota, kiek procentų skiriasi gautieji porų hash'ai:

1. bit'ų lygmeniu,
2. hex’ų lygmeniu. ir parodytos minimalios, maksimalios ir vidutines skirtingumo reikšmės.

![alt text](<nuotraukos/Screenshot 2025-10-08 231535.png>)

Rezultatas: Bitų skirtumas ir hexų skirtumas 10, 50, 100 ir 500 ilgio porose yra beveik vienodi.

Lavinos efektas išlieka efetyvus visokiuose string'ų ilgių pororse ir užtvirtina, kad nėra kolizijų.

--- 
## 6. Negrįžtamumo demonstracija
Žemiau parodytos nuotraukos, kaip hash'as dirba su HASH(input + salt).

Ir taip pat kaip reaguoja ieškomas hashas, prasidedantis iš tų pačių simbolių po 100 000 bandymų.

![alt text](<nuotraukos/Screenshot 2025-10-08 231552.png>)

![alt text](<nuotraukos/Screenshot 2025-10-08 231628.png>)

Rezultatas: Negalima žinant gautą hash'ą ir saltą atpažinti įvesties.

---  
# HASH'Ų LYGINIMAI
---
## 1. Efektyvumas

Išbandytas konstitucija.txt failas su 1, 2, 4, 8, 16, 32, 64 ir 128 eilutėmis.

Žemiau pateikta lentelė su gautais duomenimis su kiekvienu algoritmu paėmus vidurkį iš 5 bandymų.

| Eilučių kiekis            | Sujungtas | Mano | Nedos | Juliaus | Miglės | Tėjos | Gustavo | 
| :------------------------ | :------------------: | :----------------: | :-----------------: | :------------------: | :--------: | :-----: | :-----: |
| 1 eilutė                  | 0.000063 | 0.002397 | 0.000042 | 0.000009 | 0.000072 | 0.000184 | 0.033745 | 
| 2 eilutės                 | 0.000182 | 0.000094 | 0.000033 | 0.000005 | 0.000152 | 0.000014 | 0.027209 | 
| 4 eilutės                 | 0.000288 | 0.000078 | 0.000080 | 0.000007 | 0.000397 | 0.000013 | 0.026122 | 
| 8 eilutės                 | 0.000996 | 0.000110 | 0.000070 | 0.000007 | 0.000858 | 0.000018 | 0.022304 | 
| 16 eilučių                | 0.001988 | 0.000314 | 0.000194 | 0.000008 | 0.006920 | 0.000022 | 0.027462 | 
| 32 eilutės                | 0.003847 | 0.000378 | 0.000404 | 0.000013 | 0.023014 | 0.000050 | 0.036047 | 
| 64 eilutės                | 0.007708 | 0.000797 | 0.000618 | 0.000029 | 0.108808 | 0.000125 | 0.023052 | 
| 128 eilutės               | 0.018901 | 0.001989 | 0.001725 | 0.000050 | 0.691187 | 0.000193 | 0.028352 | 

---
## 2. Kolizijų paieška

Naudojamas failas collision_pairs.txt, kuriame yra po 100 000 atsitiktinių string porų, kurių ilgis yra: 10, 100, 500, 1000 simbolių.

Žemiau pateikta lentelė su gautais duomenimis kiekvieno algoritmo

| Stringo ilgis | Sujungtas | Mano | Nedos | Juliaus | Miglės | Tėjos | Gustavo | 
|---------------|:-----------------:|:-------------------:|:------------------:|:-------------------:|:---------:|:------:|:-----:|
| 10            | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 
| 100           | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 
| 500           | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 
| 1000          | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 

---
## 3. Lavinos efektas

Naudojamas failas avalanche_test_pairs.txt, kuriame yra 100 000 string porų, kurių ilgiai yra 10, 50, 100, 500 ir skiriasi atsitiktiniu vienu simboliu tarpusavyje.

Žemiau pateikta nuotrauka, kurioje suskaičiuota, kiek procentų skiriasi gautieji porų hash'ai:
1. bit'ų lygmeniu,
2. hex’ų lygmeniu.

Lentelėje surašomi kiekvieno algoritmo rezultatai su 500 ilgio stringo porom.

### Bitų lygmuo

| Hash                       | Max | Min | Vidurkis |
|----------------------------|:---:|:---:|:--------:|
| Sujungtas                  | 63.3% | 17.6% | 47.18% |
| Mano                       | 64.1% | 0.00% | 41.69% |
| Nedos                      | 61.5% | 0.00% | 45.2% |
| Juliaus                    | 63.7% | 38.7% | 50.02% |
| Miglės                     | 62.9% | 37.9% | 50.02% |
| Tėjos                      | 64.5% | 37.9% | 49.99% |
| Gustavo                    | 62.1% | 0.39% | 45% |

### Hex lygmuo

| Hash                       | Max | Min | Vidurkis |
|----------------------------|:---:|:---:|:--------:|
| Sujungtas                  | 100% | 37.5% | 88.50% |
| Mano                       | 100% | 0.00% | 78.24% |
| Nedos                      | 96.1% | 0.00% | 76.5% |
| Juliaus                    | 100% | 79.7% | 93.76% |
| Miglės                     | 100% | 78.1% | 93.76% |
| Tėjos                      | 100% | 79.7% | 93.75% |
| Gustavo                    | 100% | 1.56% |  89% |

---
# IŠVADOS:

Rezultatų lentelė:

1. Julius
2. Tėja
3. Miglė
4. Neda
5. Mano
6. Gustao
