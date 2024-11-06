# TEMA LABORATOR 1 ROBOTICA - Joc TypeRacer pe Arduino

Proiectul simulează un joc de tip TypeRacer pe platforma Arduino, în care utilizatorul tastează corect cuvintele afișate în terminal într-un timp stabilit. Acest joc include selecția dificultății, LED-uri pentru indicarea stării și două butoane: unul pentru pornirea/oprirea jocului și unul pentru selectarea dificultății.

---

## Funcționalitatea Jocului

1. **Stare de Repaus**  
   Când jocul este inactiv, LED-ul RGB va afișa culoarea albă.

2. **Selectarea Dificultății**  
   În modul de repaus, apăsarea butonului de dificultate va permite alegerea dintre trei nivele de dificultate: `Easy`, `Medium` și `Hard`. Mesajul corespunzător dificultății este afișat în terminal: **“Easy/Medium/Hard mode on!”**.

3. **Start Runda**  
   Apăsarea butonului de start în modul de repaus va porni jocul. LED-ul RGB va clipi de 3 ori, indicând o numărătoare inversă de 3 secunde până la începerea rundei.

4. **În timpul jocului**  
   - La începutul rundei, LED-ul RGB devine verde și cuvintele sunt afișate în terminal pentru a fi tastate corect.
   - Dacă utilizatorul tastează cuvântul corect, un nou cuvânt este afișat imediat.
   - Dacă timpul stabilit de dificultate expiră înainte ca utilizatorul să tasteze corect cuvântul, un nou cuvânt este afișat.
   - O greșeală va determina LED-ul să devină roșu; utilizatorul poate folosi tasta `Backspace` pentru a corecta.

5. **Oprirea Jocului**  
   Apăsarea butonului de start în timpul unei runde va opri imediat jocul și va reseta starea la repaus.

6. **Sfârșitul Rundei**  
   După 30 de secunde, runda se va încheia automat, iar scorul total de cuvinte corecte va fi afișat în terminal.

---

## Componente Utilizate

- **1x** Placă Arduino UNO (microcontroler ATmega328P)
- **1x** LED RGB pentru indicarea stării
- **2x** Butoane (unul pentru start/stop rundă, altul pentru selectarea dificultății)
- **5x** Rezistențe:
  - **3x** de 220/330 ohm pentru LED-ul RGB
  - **2x** de 1000 ohm pentru butoane
- Breadboard și fire de legătură

---

## Detalii Tehnice

### LED RGB - Indicator de Stare (1p)

- **Mod Repaus**: LED-ul RGB are culoarea albă.
- **Numărătoare Inversă**: LED-ul RGB va clipi timp de 3 secunde pentru a indica numărătoarea inversă la pornirea jocului.
- **În timpul Rundei**:
  - LED-ul RGB devine **verde** pentru un răspuns corect.
  - LED-ul RGB devine **roșu** pentru un răspuns greșit.

### Buton Start/Stop (1p)

- **Mod Repaus**: Dacă jocul este inactiv, apăsarea butonului inițiază o nouă rundă după numărătoarea inversă.
- **În timpul Rundei**: Dacă jocul este activ, apăsarea butonului îl va opri imediat.

### Buton Dificultate (1p)

- Permite ciclarea între modurile de dificultate (`Easy`, `Medium`, `Hard`) în modul de repaus.
- Mesajele corespunzătoare dificultății sunt afișate în terminal.

### Generarea Cuvintelor (3p)

- Dicționarul de cuvinte este creat pe Arduino.
- Cuvintele sunt afișate într-o ordine aleatorie în timpul jocului.
- La tastarea corectă a unui cuvânt, un alt cuvânt va apărea imediat.
- Dacă timpul stabilit de dificultate expiră, apare automat un nou cuvânt.

### Alte Detalii (1p)

- **Durata Rundei**: 30 de secunde.
- **Scor Final**: La finalul fiecărei runde, scorul (numărul total de cuvinte corecte) este afișat în terminal.

---

## Fluxul Jocului

1. **Modul de Repaus**  
   LED-ul RGB are culoarea albă.

2. **Alegerea Dificultății**  
   Butonul de dificultate permite selectarea modului dorit, iar în terminal este afișat mesajul corespunzător.

3. **Apăsarea Start**  
   LED-ul RGB va clipi timp de 3 secunde, indicând o numărătoare inversă până la începerea rundei.

4. **Runda Activă**  
   LED-ul devine verde, iar cuvintele sunt afișate în terminal pentru a fi tastate corect.
   - **Cuvânt Corect**: LED-ul RGB rămâne verde, iar un nou cuvânt este afișat imediat.
   - **Cuvânt Greșit**: LED-ul RGB devine roșu, iar utilizatorul poate corecta textul folosind `Backspace`.

5. **Finalul Rundei**  
   După 30 de secunde, runda se încheie automat și este afișat scorul final, reprezentând numărul de cuvinte tastate corect.

6. **Oprirea Jocului**  
   Jocul se poate opri oricând prin apăsarea butonului de start/stop, iar LED-ul RGB va reveni la culoarea albă, indicând starea de repaus.

---

### Schema Setup Fizic

- **Schema breadboard fizică**: [Schema breadboard](https://drive.google.com/file/d/1CqBopthb3kALGBSLGa4HUZLE6jmcNxF0/view?usp=drive_linkk)
[Schema breaboard]( https://drive.google.com/file/d/1motyAh25T5dkghyg6k2TlFVKarAm2FbO/view?usp=drive_link)

---

### Schena wowKI
[Schema] (https://wokwi.com/projects/413816682527998977)
---

### Video Funcționare

- **Demonstrație video a funcționării**: [Link către video](https://youtube.com/shorts/gYVs_Wss-xI?feature=share)
