# Kompresija Izvora - Hafmenov Algoritam i LZV u C++-u

Konzolna aplikacija u C++-u koja implementira kompresiju binarnih sekvenci generisanih **Markovljevim izvorom** korišćenjem dva algoritma: **Hafmenovog kodovanja** (sa prosirenjima do 5. reda) i **LZV (Lempel-Ziv-Welch) kompresije**.

# Opis programa

Program na osnovu zadatih verovatnoća prelaza Markovljevog lanca generiše binarnu sekvencu od 10.000 simbola, a zatim nad njom primenjuje i poredi različite metode kompresije:

1. **Generisanje sekvence** - Markovljev izvor sa dva stanja (0 i 1) i zadatim verovatnoćama prelaza p₀ (0→1) i p₁ (1→0)
2. **Hafmenovo kodovanje** - standardno kodovanje simbola 0 i 1 na osnovu izmerenih verovatnoća iz generisane sekvence
3. **Hafmenovo kodovanje sa prosirenjima** - 2., 3., 4. i 5. prosirenje (kombinacije simbola kao novi simboli abecede)
4. **LZV kompresija** - Lempel-Ziv-Welch algoritam sa rečnikom koji se gradi tokom kompresije
5. **Poređenje stepena kompresije** - odnos veličine originalne i kompresovane sekvence za svaku metodu

# Arhitektura projekta

| Fajl | Sadržaj |
|------|---------|
| `stablo.h / stablo.cpp` | Binarno stablo za Hafmenovo kodovanje (`Cvor`, `Stablo`) |
| `red.h / red.cpp` | Prioritetni red (sortirana ulančana lista) za izgradnju stabla |
| `recnik.h / recnik.cpp` | Rečnik za LZV algoritam sa kopija-konstruktorom i move-konstruktorom |
| `main.cpp` | Generisanje sekvence, poziv algoritama, ispis rezultata |

# Algoritmi

# Hafmenovo kodovanje
- Svi simboli se umeću u **prioritetni red** sortiran po verovatnoći
- Svaka iteracija: vade se 2 simbola sa najmanjom verovatnoćom, spajaju u novo čvor-stablo, vraćaju u red
- Po završetku BFS obilaskom stabla svaki list dobija kodnu reč (levo = `0`, desno = `1`)

# Prosirenje reda n
- Iz polaznog skupa simbola (0, 1) generišu se sve kombinacije dužine n (npr. za n=2: 00, 01, 10, 11)
- Verovatnoća kombinacije = proizvod verovatnoća pojedinih simbola
- Hafmenov algoritam se primenjuje na ovaj prošireni skup

# LZV kompresija
- Rečnik se inicijalizuje sa simbolima „0" i „1"
- Sekvenca se procesira karakter po karakter; svaka nova kombinacija koja nije u rečniku dodaje se u rečnik
- Izlaz: niz adresa iz rečnika konvertovanih u binarni oblik

# Ulazni parametri

Program na startu traži:
```
Unesite verovatnoce prelaska iz stanja 1 u 0 i iz stanja 0 u 1:
p1 p0
```

Primer: `0.1 0.2` znači da je verovatnoća prelaza 1→0 jednaka 0.1, a 0→1 jednaka 0.2.

# Pokretanje

# Zahtevi
- Windows operativni sistem
- Visual Studio (preporučuje se 2022)

# Koraci
1. Otvori `pmt.sln` u Visual Studio-u
2. Kompajliraj projekat (`Ctrl+Shift+B`)
3. Pokreni sa `Ctrl+F5`
4. Unesi verovatnoće prelaza Markovljevog lanca

# Struktura projekta

```
pmt/
├── pmt.sln
└── pmt/
    ├── main.cpp
    ├── stablo.h / stablo.cpp
    ├── red.h / red.cpp
    └── recnik.h / recnik.cpp
```


