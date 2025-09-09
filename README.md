# Calculator_avansat


Este un proiect scris in limbajul C care implementeaza un calculator stiintific extensibil 
Proiectul combina : 
  - un motor matematic ce poate interpreta si evalua expresii
  - functii matematice avansate(sin,cos,log,sqrt, etc)
  - si o interfata grafica facuta cu SDL2 cu butoane si afisaj

Proiectul este conceput modular : intai logica in consola, apoi interfata grafica 


Funtionalitatea implementata 
  1. Operatii aritmetice de baza : +,-,*,/
  2. Paranteze pentru ordinea operatiilor
  3. Funcii matematice :
      -sin(x)
      -cos(x)
      -sqrt(x)
      -log(x)
  4.Conversii temperatura :
      -CtoF(x) - Celsius -> Fahrenheit
      -FtoC(x) - Fahrenheit -> Celsius
  5.Conversii unghiuri :
      -DEG(x) - Grade -> Radiani
      -RAD(x) - Radiani -> Grade
  6.Conversii numerice :
      -BIN(X) - Afisare in binar
      -HEX(X) - Afisare in hexazecimal

Pasii proiectului (roadmap) : 

  1. Motor matematic simplu (infix -> postfix -> evaluare) 
  2. Functii matematice + conversii utile 
  3. Istoric calcule + memorie
  4. Interfata grafica SDL2
  5. Extensii optionale

Tehnologii folosite : 
  -C
  -Algoritmul Shunting Yard
  -Stive
  -Math.h
  -SDL2

Posibile imbunatatiri : 
  -suport pentru functii trigonometrice inverse
  -suport pentru pow(x,y) / ridicarea la putere
  -mod stiintific complet(constante pi, e, etc)
  -modul grafic cu design asemanator unui calculator real
