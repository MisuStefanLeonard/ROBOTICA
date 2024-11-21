# ⁠Joc Competitiv 1 vs 1: Master vs Slave

Acest proiect implementează un joc competitiv între doi jucători (Master și Slave), în care aceștia trebuie să apese butonul corespunzător culorii afișate pe LED-ul RGB, pentru a obține puncte. Jocul se desfășoară pe două plăci Arduino Uno, comunicând prin SPI, și include un servomotor care indică progresul jocului, iar punctajul fiecărui jucător este afișat pe un LCD.

## Funcționalitate

Jocul are două moduri de interacțiune principale: fiecare jucător apasă butonul corespunzător culoarei LED-ului RGB care se aprinde. Jucătorii concurează pentru a obține cel mai mare punctaj, iar progresul jocului este indicat de un servomotor care se rotește. La final, câștigătorul este afișat pe LCD.

## Componente Utilizate

- **2x Arduino Uno**: Unul pentru Master și unul pentru Slave, care comunică prin SPI.
- **3x Breadboard-uri**:
  - **Master Breadboard**: Conține 3 butoane, 3 LED-uri, 1 LED RGB, LCD și servomotor.
  - **Slave Breadboard**: Conține 3 butoane, 3 LED-uri și 1 LED RGB.
  - **Al 3-lea Breadboard**: Conține butonul de start, LCD-ul și servomotorul.
- **6x LED-uri**: 3 pentru fiecare jucător, fiecare asociat unui buton.
- **2x LED RGB**: Câte unul pentru fiecare jucător, care se aprind în culoarea corespunzătoare butonului de apăsat.
- **6x Butoane**: Câte 3 pentru fiecare jucător.
- **1x LCD**: Afișează punctajul și starea jocului.
- **1x Servomotor**: Indică progresul jocului.
- **Fire de legătura**
- **Rezistoare pentru butoane și LED-uri**

## Flow-ul Jocului

1. **Inițializare**:
   - La pornirea jocului, LCD-ul afișează un mesaj de bun venit.
   - Apăsarea unui buton de start (pe al 3-lea breadboard) va începe jocul.

2. **Desfășurarea rundelor**:
   - La fiecare rundă, un LED RGB se aprinde pe fiecare plăcuță (Master și Slave) într-o culoare aleatorie.
   - Jucătorul trebuie să apese butonul corespunzător acelei culori.
   - Dacă butonul apăsat este corect, jucătorul câștigă puncte. Cu cât reacționează mai repede, cu atât primește mai multe puncte.
   - Punctajul actualizat al ambilor jucători este afișat pe LCD.

3. **Progresul Jocului**:
   - Servomotorul se rotește pe măsură ce jocul progresează.
   - La finalul jocului, LCD-ul va afișa câștigătorul și scorul final pentru câteva secunde, apoi revine la mesajul de bun venit.

4. **Finalizarea Jocului**:
   - Jocul se termină după un anumit număr de runde sau când servomotorul face o rotație completă.
   - LCD-ul afișează câștigătorul și scorul final.

## Detalii Tehnice

### 1. **SPI Communication**:
   - Utilizăm două plăci Arduino Uno conectate prin SPI pentru a permite comunicarea între Master și Slave.
   - Master-ul controlează LCD-ul și servomotorul, și ține evidența punctajului și stării jocului.
   - Slave-ul controlează butoanele și LED-urile sale, primind instrucțiuni de la Master și trimițând înapoi starea butoanelor apăsate.

### 2. **Butoane**:
   - **Butonul de Start** este implementat pe al 3-lea breadboard. Apăsarea acestuia va începe jocul.
   - Butoanele fiecărui jucător sunt asociate cu LED-uri de culori diferite (roșu, albastru, verde).

### 3. **LED-uri RGB**:
   - Fiecare jucător are un LED RGB asociat care se aprinde într-o culoare corespunzătoare butonului ce trebuie apăsat.

### 4. **LCD**:
   - LCD-ul afișează punctajul curent al fiecărui jucător.
   - Folosește biblioteca `LiquidCrystal` pentru a afişa textul clar pe display.

### 5. **Servomotor**:
   - Servomotorul se rotește pe măsură ce timpul progresează în joc.
   - La sfârșitul fiecărei runde, servomotorul indică finalul jocului printr-o rotație completă.

## Mod de Funcționare

1. **La început**: LCD-ul va arăta un mesaj de bun venit. Servomotorul începe de la 0 grade.
2. **După apăsarea butonului de start**:
   - Se va genera o culoare aleatorie pe LED-ul RGB al fiecărui jucător.
   - Jucătorii trebuie să apese butonul corespunzător acestei culori pentru a câștiga puncte.
3. **Pe parcursul jocului**:
   - LCD-ul va actualiza punctajul jucătorilor.
   - Servomotorul va indica progresul jocului.
4. **La final**:
   - LCD-ul va afișa câștigătorul și scorul final pentru câteva secunde.
   - Servomotorul se oprește după o rotație completă.

## Secvențe de Blink

- **LED-urile jucătorilor**: La începutul fiecărei runde, LED-urile jucătorilor se aprind pentru a semnaliza culoarea corespunzătoare butonului.
- **LED-ul RGB**: Se aprinde în culoarea corespunzătoare butonului ce trebuie apăsat.
  
## Video Funcționalitate

*(https://youtube.com/shorts/YdtFgcG7viQ?feature=share)*

## Poze

*(https://drive.google.com/drive/folders/1O9qiiJqPC3hQiVrvW6cKnFdu7PfPgtkx)*