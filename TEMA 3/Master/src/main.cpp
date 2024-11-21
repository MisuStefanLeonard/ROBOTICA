#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Definirea pinilor pentru componente
#define START_BUTTON 8
#define MASTER_YELLOW 2
#define MASTER_BLUE 3
#define MASTER_GREEN 4
#define MASTER_RGB_RED 7
#define MASTER_RGB_GREEN 6
#define MASTER_RGB_BLUE 5

Servo myServo;  // Obiect Servo pentru controlul mișcării
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Inițializare LCD

// Prototipuri de funcții utilizate
void setLed(int red, int green, int blue);
void countdown();
void startGame();
void resetSlaveFlags();
int lightRandomColour();
unsigned long receiveChar();
char receiveCharFromSlave();
void displayWinner();
void turnOffRgbSlave();

// Variabile pentru debouncing și gestionarea scorului
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int buttonState = 0;
int lastButtonState = 0;
unsigned long startRoundTime = 0;
int colorIndexMaster = 10;
bool CHECK_RGB_LIGHTEN_MASTER = true;
unsigned long lastTime = 0;
unsigned long userMasterScore = 0;
unsigned long userSlaveScore = 0;
int userMasterTotalScore = 0;
int userSlaveTotalScore = 0;
unsigned long slaveMillis = 0;
bool userSlaveScoreFlag = true;
bool userMasterScoreFlag = true;
int servoRotation = 0;
char slaveChar = 'N';

void setup() {
  myServo.attach(9);  // Atașează servo-ul la pinul digital 9
  Serial.begin(9600); // Inițializare serială pentru debugging
  SPI.begin();        // Pornire modul SPI
  pinMode(A4, INPUT); 
  pinMode(A5, INPUT); 
  pinMode(START_BUTTON, INPUT_PULLUP); // Buton cu rezistor pull-up
  SPI.setClockDivider(SPI_CLOCK_DIV8); // Configurează frecvența SPI

  // Configurarea pinilor pentru LED-uri
  pinMode(MASTER_YELLOW, OUTPUT);
  pinMode(MASTER_BLUE, OUTPUT);
  pinMode(MASTER_GREEN, OUTPUT);
  pinMode(MASTER_RGB_RED, OUTPUT);
  pinMode(MASTER_RGB_GREEN, OUTPUT);
  pinMode(MASTER_RGB_BLUE, OUTPUT);
  
  lcd.init();  // Inițializarea LCD-ului
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Bun venit!");  // Mesaj de bun venit pe LCD

  // Pornirea LED-urilor normale pentru semnalizare inițială
  digitalWrite(MASTER_YELLOW, HIGH);
  digitalWrite(MASTER_BLUE, HIGH);
  digitalWrite(MASTER_GREEN, HIGH);

  randomSeed(675454); // Inițializarea generatorului de numere aleatorii
  myServo.write(0);   // Servo-ul pornește de la poziția 0
}

void loop() {
  // Debouncing pentru buton
  int reading = digitalRead(START_BUTTON);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == 0) {
      buttonState = 1;  // Marcare că butonul a fost apăsat
      Serial.println("Start Game!");
      startGame();  // Pornirea jocului
    }
  }

  if (reading == HIGH) {
    buttonState = 0; // Resetare stare buton
  }

  lastButtonState = reading; // Actualizare stare precedentă buton
}

// Funcție pentru numărătoarea inversă
void countdown() {
  for (int i = 3; i > 0; i--) {
    lcd.clear();                     
    lcd.setCursor(0, 0);             
    lcd.print("Incepe runda in");    
    lcd.setCursor(0, 1);             
    lcd.print(i);                    
    delay(1000);                     
  }
}

// Configurare LED RGB cu valori de intensitate
void setLed(int red, int green, int blue) {
  analogWrite(MASTER_RGB_RED, red);
  analogWrite(MASTER_RGB_GREEN, green);
  analogWrite(MASTER_RGB_BLUE, blue);
}

// Funcție principală pentru rularea jocului
void startGame() {
  for (int i = 0; i < 5; i++) { // Joc format din 5 runde
    setLed(0, 0, 0);  // Oprește RGB-ul la începutul rundei
    turnOffRgbSlave(); 
    countdown(); 
    resetSlaveFlags(); // Resetare flaguri pe slave
    lcd.clear(); 
    colorIndexMaster = -1;
    CHECK_RGB_LIGHTEN_MASTER = false;
    startRoundTime = millis();
    lastTime = millis();
    userMasterScore = 0;
    userSlaveScore = 0;
    slaveChar = 'N';
    userSlaveScoreFlag = true;
    userMasterScoreFlag = true;

    while (millis() - startRoundTime <= 3000) {
      // Controlează rotația servo-ului
      if (millis() - lastTime >= 250) {
        servoRotation += 3;
        myServo.write(servoRotation);
        lastTime = millis();
      }

      // Alege o culoare aleatorie pentru Master
      if (colorIndexMaster == -1) {
        colorIndexMaster = lightRandomColour();
        Serial.println(colorIndexMaster);
      }

      // Verifică butonul apăsat pentru Master
      if (CHECK_RGB_LIGHTEN_MASTER == false) {
        int value = analogRead(A1);
        if (colorIndexMaster == 0 && value >= 800 && value < 900) {
          Serial.println("APASAT CORECT ROSU");
          CHECK_RGB_LIGHTEN_MASTER = true;
          userMasterScore = millis();
        } else if (colorIndexMaster == 1 && value >= 600 && value < 790) {
          Serial.println("APASAT CORECT ALBASTRU");
          CHECK_RGB_LIGHTEN_MASTER = true;
          userMasterScore = millis();
        } else if (colorIndexMaster == 2 && value >= 5 && value < 50) {
          Serial.println("APASAT CORECT VERDE");
          CHECK_RGB_LIGHTEN_MASTER = true;
          userMasterScore = millis();
        }
      }

      // Obține timpul de la slave
      slaveChar = receiveCharFromSlave();
      if (slaveChar == 'Y') {
        userSlaveScore = millis();
      }

      // Actualizează scorurile dacă au fost apăsate corect
      if (userSlaveScore != 0 && userSlaveScoreFlag) {
        userSlaveTotalScore += (3000 - (userSlaveScore - startRoundTime)) / 100;
        userSlaveScoreFlag = false;
      }
      if (userMasterScore != 0 && userMasterScoreFlag) {
        userMasterTotalScore += (3000 - (userMasterScore - startRoundTime)) / 100;
        userMasterScoreFlag = false;
      }

      // Actualizează scorurile pe LCD
      lcd.setCursor(0, 0);
      lcd.print("P1: ");
      lcd.print(userMasterTotalScore);
      lcd.print("    "); 

      lcd.setCursor(0, 1);
      lcd.print("P2: ");
      lcd.print(userSlaveTotalScore);
      lcd.print("    "); 
    }

    servoRotation += 3; // Finalizează rotația servo-ului
    myServo.write(servoRotation);
  }

  displayWinner(); // Afișează câștigătorul
  delay(3000);
  myServo.write(0);
  servoRotation = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bun venit!");
  userSlaveTotalScore = userMasterTotalScore = 0;
}

// Resetarea flagurilor slave-ului
void resetSlaveFlags() {
  digitalWrite(SS, LOW);
  SPI.transfer(0x01);     
  digitalWrite(SS, HIGH); 
}

// Stingerea RGB-ului de pe slave
void turnOffRgbSlave() {
  digitalWrite(SS, LOW);
  SPI.transfer(0x02);
  digitalWrite(SS, HIGH);
}

// Selectare culoare aleatorie
int lightRandomColour() {
  int colorIndex = random(0, 3);
  switch (colorIndex) {
    case 0: setLed(255, 0, 0); break;
    case 1: setLed(0, 0, 255); break;
    case 2: setLed(0, 255, 0); break;
  }
  return colorIndex;
}

// Obținere caracter de la slave prin SPI
char receiveCharFromSlave() {
  char result = 'N';
  digitalWrite(SS, LOW);
  SPDR = 0x00; 
  while (!(SPSR & _BV(SPIF))); 
  result = SPDR;  
  digitalWrite(SS, HIGH);
  return result;
}

// Afișează câștigătorul pe LCD
void displayWinner() {
  lcd.clear();
  if (userMasterTotalScore > userSlaveTotalScore) {
    lcd.setCursor(0, 0);
    lcd.print("Castigator: P1");
  } else if (userSlaveTotalScore > userMasterTotalScore) {
    lcd.setCursor(0, 0);
    lcd.print("Castigator: P2");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Egalitate!");
  }

  lcd.setCursor(0, 1);
  lcd.print("P1: ");
  lcd.print(userMasterTotalScore);
  lcd.print("  P2: ");
  lcd.print(userSlaveTotalScore);

  delay(3000); 
}
