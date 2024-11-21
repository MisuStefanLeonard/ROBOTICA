#include <SPI.h>
#include <Arduino.h>

// Definirea pinilor pentru LED-uri și RGB LED
#define SLAVE_RED 4
#define SLAVE_BLUE 2
#define SLAVE_GREEN 3
#define SLAVE_RGB_RED 7
#define SLAVE_RGB_GREEN 6
#define SLAVE_RGB_BLUE 5

// Variabile pentru gestionarea stării LED-urilor și a culorilor
bool CHECK_RGB_LIGHTEN = true; // Indică dacă RGB a fost aprins
int colorIndex = 10;           // Index pentru culoarea curentă
int val = 0;                   // Valoarea analogică citită
bool rgbOffFlag = false;       // Indică dacă LED-ul RGB a fost stins

// Prototipurile funcțiilor utilizate
void setLed(int red, int green, int blue); // Configurarea culorii LED-ului RGB
int lightRandomColour();                  // Selectarea aleatorie a unei culori
void sendCharToMaster(char dataToSend);   // Trimiterea unui caracter către Master

void setup() {
  pinMode(MISO, OUTPUT);          // Configurarea pinului MISO ca ieșire
  SPCR |= _BV(SPE);               // Activarea modulului SPI în modul Slave
  SPI.attachInterrupt();          // Atașarea întreruperii SPI
  Serial.begin(9600);             // Inițializarea serială pentru debugging
  
  // Configurarea pinilor pentru LED-uri ca ieșire
  pinMode(SLAVE_RED, OUTPUT);
  pinMode(SLAVE_BLUE, OUTPUT);
  pinMode(SLAVE_GREEN, OUTPUT);
  pinMode(SLAVE_RGB_RED, OUTPUT);
  pinMode(SLAVE_RGB_GREEN, OUTPUT);
  pinMode(SLAVE_RGB_BLUE, OUTPUT);
  
  // Pornirea LED-urilor normale ca semnalizare inițială
  digitalWrite(SLAVE_RED, HIGH);
  digitalWrite(SLAVE_BLUE, HIGH);
  digitalWrite(SLAVE_GREEN, HIGH);

  randomSeed(56); // Inițializarea generatorului aleatoriu cu un seed fix
}

// ISR declanșată de transferul SPI
ISR(SPI_STC_vect) {
  char receivedData = SPDR; // Datele primite de la Master
  if (receivedData == 0x01) { 
    // Resetarea flagurilor pentru o nouă rundă
    colorIndex = -1;
    CHECK_RGB_LIGHTEN = false;
    rgbOffFlag = false;
    Serial.println("Flaguri resetate!");
  } else if (receivedData == 0x02) {
    // Stinge LED-ul RGB la comanda Masterului
    rgbOffFlag = true;
    setLed(0, 0, 0);
    Serial.println("RGB STINS!");
  }
  SPDR = receivedData; // Trimite înapoi datele primite (ecou)
}

void loop() {
  if (colorIndex == -1) {
    // Alege o culoare aleatorie dacă flagul indică o schimbare
    colorIndex = lightRandomColour();
    Serial.println(colorIndex);
  }

  if (CHECK_RGB_LIGHTEN == false) {
    // Citește valoarea analogică și verifică dacă este apăsat butonul corect
    int value = analogRead(A1);

    if (colorIndex == 0 && value >= 250 && value < 300) {
      // Detectează apăsarea pentru culoarea roșie
      Serial.println("APASAT CORECT ROSU");
      CHECK_RGB_LIGHTEN = true;
      sendCharToMaster('Y'); // Trimite confirmarea către Master
    } else if (colorIndex == 1 && value >= 0 && value < 50) {
      // Detectează apăsarea pentru culoarea albastră
      Serial.println("APASAT CORECT ALBASTRU");
      CHECK_RGB_LIGHTEN = true;
      sendCharToMaster('Y'); // Trimite confirmarea către Master
    } else if (colorIndex == 2 && value >= 100 && value < 200) {
      // Detectează apăsarea pentru culoarea verde
      Serial.println("APASAT CORECT VERDE");
      CHECK_RGB_LIGHTEN = true;
      sendCharToMaster('Y'); // Trimite confirmarea către Master
    }
  }
}

void sendCharToMaster(char dataToSend) {
  // Trimite un singur caracter către Master prin SPI
  SPDR = dataToSend; // Scrie caracterul în registrul SPI Data Register
  while (!(SPSR & _BV(SPIF))); // Așteaptă finalizarea transferului SPI
}

void setLed(int red, int green, int blue) {
  // Configurează intensitatea fiecărui canal al LED-ului RGB
  analogWrite(SLAVE_RGB_RED, red);
  analogWrite(SLAVE_RGB_GREEN, green);
  analogWrite(SLAVE_RGB_BLUE, blue);
}

int lightRandomColour() {
  // Selectează aleatoriu o culoare și setează LED-ul RGB corespunzător
  int colorIndex = random(0, 3);
  switch (colorIndex) {
    case 0:
      setLed(255, 0, 0); // Roșu
      break;
    case 1:
      setLed(0, 0, 255); // Albastru
      break;
    case 2:
      setLed(0, 255, 0); // Verde
      break;
  }
  return colorIndex; // Returnează indexul culorii alese
}
