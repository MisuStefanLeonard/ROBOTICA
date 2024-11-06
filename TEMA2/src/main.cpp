#include "words.h"


void ledColor(int red, int green, int blue){
  analogWrite(RED_RGB_PIN, red);
  analogWrite(GREEN_RGB_PIN, green);
  analogWrite(BLUE_RGB_PIN, blue);
}


void stopRound() {
  Serial.println("Runda s-a terminat!");
  Serial.print("Ai nimerit ");
  Serial.print(pointsCount);
  Serial.println(" cuvinte corecte!");
  ledColor(0, 255, 0); 
  for (int i = 0; i < 7; i++) {
    freqWords[i] = 0; 
  }
}


void changeDifficulty() {
  counter++; 
  switch (counter % 3) {
    case 0:
      setDifficulty = HARD;
      timePerWord = 2000;
      Serial.println("Dificultate: HARD");
      break;
    case 1:
      setDifficulty = EASY;
      timePerWord = 4000;
      Serial.println("Dificultate: EASY");
      break;
    case 2:
      setDifficulty = MEDIUM;
      timePerWord = 3000;
      Serial.println("Dificultate: MEDIUM");
      break;
  }
}


int generateWordIndex(){
  int randomIndex;
  do {
    randomIndex = random(0, sizeof(cuvinteJoc) / sizeof(cuvinteJoc[0]));
  } while (freqWords[randomIndex] == 1);

  freqWords[randomIndex] = 1;
  return randomIndex;
}

void execRound() {
  pointsCount = 0;
  userTime = 0;

  while (userTime < timeRound) {
    String toStringWord = cuvinteJoc[generateWordIndex()];
    ledColor(0,255,0);
    String inputWord = ""; 
    Serial.print("Cuvant generat: ");
    Serial.println(toStringWord);

    wordStartTime = millis(); 
    unsigned long currentTime;

    while ((millis() - wordStartTime) < timePerWord) {
      currentTime = millis() - wordStartTime;

      if (userTime + currentTime > timeRound) {
        stopRound();
        return;
      }

      if (Serial.available() > 0) {
        char receivedChar = Serial.read();
        if (receivedChar != -1) { 
          if (receivedChar == '\b' && inputWord.length() > 0) {
            inputWord.remove(inputWord.length() - 1);
          } else {
            inputWord += receivedChar;
          }

         
          if (toStringWord.startsWith(inputWord)) {
            ledColor(0, 255, 0); 
          } else {
            ledColor(255, 0, 0); 
          }

         
          if (toStringWord == inputWord) {
            pointsCount++;
            break;
          }
        }
      }
    }
    userTime += currentTime; 
  }
  stopRound();
}


void startRound(){
  if (millis() - countdownStartTime >= 1000 && countdownActive) { 
    Serial.print("Jocul va incepe in ");
    Serial.println(countdownValue);
    countdownValue--; 
    countdownStartTime = millis(); 

    if (countdownValue <= 0) {
      Serial.println("Jocul a inceput!");
      countdownActive = false; 
      countdownValue = 3; 
      execRound(); 
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RED_RGB_PIN, OUTPUT);
  pinMode(GREEN_RGB_PIN, OUTPUT);
  pinMode(BLUE_RGB_PIN, OUTPUT);
  pinMode(DIFFICULTY_BUTTON, INPUT_PULLUP);
  pinMode(START_BUTTON, INPUT_PULLUP);

  
  ledColor(0, 255, 0); // Green LED for easy
}

void loop() {
  bool buttonState = digitalRead(DIFFICULTY_BUTTON);
  bool startButtonState = digitalRead(START_BUTTON);

  // Handle debounce for difficulty button
  if (buttonState != stareUltimButon) {
    ultimaDebounceDifficulty = millis();
  }

  // Handle debounce for start button
  if (startButtonState != stareUltimButonStart) {
    ultimaDebounceStart = millis();
  }

  // Difficulty button press handling with debounce
  if ((millis() - ultimaDebounceDifficulty) > delayDebounce) {
    if (buttonState == LOW && buttonPressed == false) {
      changeDifficulty();
      buttonPressed = true;
    }
    if(buttonState == HIGH) {
      buttonPressed = false;
    }
  }
  stareUltimButon = buttonState;

  // Start button press handling with debounce
  if ((millis() - ultimaDebounceStart) > delayDebounce) {
    if (buttonPressedStart == LOW && buttonPressedStart == false) {
      countdownActive = true;
      buttonPressedStart = true;
    }
    if(startButtonState == HIGH) {
      buttonPressedStart = false;
    }
  }
  stareUltimButonStart = buttonPressedStart;

  // Start the countdown if active
  if (countdownActive) {
    startRound();
  }
}
