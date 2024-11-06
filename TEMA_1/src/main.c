#include <Arduino.h>

#define LED_ALBASTRU_2 10
#define LED_GALBEN_2 9
#define LED_GALBEN_1 8
#define LED_ALBASTRU_1 7
#define LED_VERDE_RGB 5
#define LED_ALBASTRU_RGB 4
#define LED_ROSU_RGB 6
#define BUTON_START 2
#define BUTON_STOP 3

int stareButonCurenta = HIGH;         // Starea curentă a butonului
int stareUltimButon = HIGH;           // Starea precedentă a butonului
unsigned long ultimaDebounce = 0;     // Ultima dată când a fost schimbată starea
unsigned long delayDebounce = 50;     // Timpul de debounce în milisecunde

bool incarcareInDesfasurare = false;  // Indică dacă încărcarea este în desfășurare
unsigned long momentApasareStop = 0;  // Momentul în care a fost apăsat butonul stop
bool butonStopApasatLung = false;     // Indică dacă butonul stop a fost apăsat mai mult de 1 secundă
unsigned long timpUltimBlink = 0;     // Timp pentru a gestiona intervalele de blink


// Declarații funcții
void pornireIncarcare();
void oprireIncarcare();
void verificaButonStop();

void setup() {
  // Configurare pinii butoanelor ca input cu rezistență internă pull-up
  pinMode(BUTON_START, INPUT_PULLUP);
  pinMode(BUTON_STOP, INPUT_PULLUP);

  // Configurare pinii LED-urilor ca output
  pinMode(LED_GALBEN_1, OUTPUT);
  pinMode(LED_GALBEN_2, OUTPUT);
  pinMode(LED_GALBEN_1, OUTPUT);
  pinMode(LED_ALBASTRU_1, OUTPUT);
  pinMode(LED_ROSU_RGB, OUTPUT);
  pinMode(LED_VERDE_RGB, OUTPUT);
  pinMode(LED_ALBASTRU_RGB, OUTPUT);

  // Inițializarea LED-urilor RGB
  analogWrite(LED_ROSU_RGB, 0);    // Roșu oprit
  analogWrite(LED_VERDE_RGB, 255); // Verde aprins
  analogWrite(LED_ALBASTRU_RGB, 0); // Albastru oprit
}

void loop() {
  // Citirea stării butonului de start
  int citire = digitalRead(BUTON_START);

  // Verifică dacă starea butonului s-a schimbat (datorită zgomotului sau apăsării reale)
  if (citire != stareUltimButon) {
    ultimaDebounce = millis(); // Resetare temporizator debounce
  }
  // 00010011
  // Așteaptă timpul de debounce și confirmă schimbarea stării
  if ((millis() - ultimaDebounce) > delayDebounce) {
    if (citire != stareButonCurenta) {
      stareButonCurenta = citire;

      // Execută doar dacă butonul este apăsat (stare LOW)
      if (stareButonCurenta == LOW) {
        pornireIncarcare(); // Apelare funcție pentru pornirea încărcării
      }
    }
  }

  stareUltimButon = citire; // Salvarea citirii actuale ca stare precedentă
}

void pornireIncarcare() {
  incarcareInDesfasurare = true; // Setează flag-ul de încărcare

  // Setează LED-ul RGB la roșu
  analogWrite(LED_ROSU_RGB, 255);  // Roșu aprins
  analogWrite(LED_VERDE_RGB, 0);   // Verde oprit
  analogWrite(LED_ALBASTRU_RGB, 0);

  unsigned long timpUltimBlink = millis(); // Inițializarea timpului pentru blink
  unsigned long intervalBlink = 750; // Intervalul de blink

  // Animație de încărcare continuă
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(LED_ALBASTRU_2 - j, HIGH);
      // Blink non-blocant folosind millis()
      while (millis() - timpUltimBlink < intervalBlink) {
        verificaButonStop(); // Verifică dacă butonul stop este apăsat și menținut
        if (butonStopApasatLung) return; // Dacă butonul stop a fost menținut, ieșire din încărcare
      }

      timpUltimBlink = millis(); // Actualizare timp ultim blink
      digitalWrite(LED_ALBASTRU_2 - j, LOW);

      // Așteaptă din nou fără a bloca
      while (millis() - timpUltimBlink < intervalBlink) {
        verificaButonStop(); // Verifică dacă butonul stop este apăsat și menținut
        if (butonStopApasatLung) return;
      }

      timpUltimBlink = millis(); 
      digitalWrite(LED_ALBASTRU_2 - j, HIGH);
    }
  }

  // Oprește toate LED-urile
  digitalWrite(LED_ALBASTRU_2, LOW);
  digitalWrite(LED_GALBEN_2, LOW);
  digitalWrite(LED_GALBEN_1, LOW);
  digitalWrite(LED_ALBASTRU_1, LOW);

  // Blink pentru toate LED-urile de 3 ori
  for (int i = 0; i < 3; i++) {
    while (millis() - timpUltimBlink < intervalBlink) {
      verificaButonStop(); // Verifică dacă butonul stop este apăsat
      if (butonStopApasatLung) return;
    }

    timpUltimBlink = millis(); 
    digitalWrite(LED_ALBASTRU_2, HIGH);
    digitalWrite(LED_GALBEN_2, HIGH);
    digitalWrite(LED_GALBEN_1, HIGH);
    digitalWrite(LED_ALBASTRU_1, HIGH);

    while (millis() - timpUltimBlink < intervalBlink) {
      verificaButonStop();
      if (butonStopApasatLung) return;
    }

    timpUltimBlink = millis(); 
    digitalWrite(LED_ALBASTRU_2, LOW);
    digitalWrite(LED_GALBEN_2, LOW);
    digitalWrite(LED_GALBEN_1, LOW);
    digitalWrite(LED_ALBASTRU_1, LOW);
  }

  // Setează LED-ul RGB la verde
  analogWrite(LED_ROSU_RGB, 0);
  analogWrite(LED_VERDE_RGB, 255); 
  analogWrite(LED_ALBASTRU_RGB, 0);
}

void verificaButonStop() {
  int citireButonStop = digitalRead(BUTON_STOP);

  // Dacă butonul stop este apăsat
  if (citireButonStop == LOW) {
    if (momentApasareStop == 0) {
      // Înregistrează timpul când a fost apăsat
      momentApasareStop = millis();
    } else {
      // Verifică dacă butonul a fost apăsat pentru mai mult de 1 secundă
      if (millis() - momentApasareStop >= 1000) {
        butonStopApasatLung = true;
        oprireIncarcare(); 
      }
    }
  } else {
    momentApasareStop = 0;
    butonStopApasatLung = false;
  }
}

void oprireIncarcare() {
  incarcareInDesfasurare = false; 
  digitalWrite(LED_ALBASTRU_2, LOW);
  digitalWrite(LED_ALBASTRU_1, LOW);
  digitalWrite(LED_GALBEN_2, LOW);
  digitalWrite(LED_GALBEN_1, LOW);

  // Blink pentru toate LED-urile de 3 ori
  for (int i = 0; i < 3; i++) {
    delay(750);
    digitalWrite(LED_ALBASTRU_2, HIGH);
    digitalWrite(LED_GALBEN_2, HIGH);
    digitalWrite(LED_GALBEN_1, HIGH);
    digitalWrite(LED_ALBASTRU_1, HIGH);
    delay(750);
    digitalWrite(LED_ALBASTRU_2, LOW);
    digitalWrite(LED_GALBEN_2, LOW);
    digitalWrite(LED_GALBEN_1, LOW);
    digitalWrite(LED_ALBASTRU_1, LOW);
  }

  // Setează LED-ul RGB la verde
  analogWrite(LED_ROSU_RGB, 0);
  analogWrite(LED_VERDE_RGB, 255);
  analogWrite(LED_ALBASTRU_RGB, 0);
}
