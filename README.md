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