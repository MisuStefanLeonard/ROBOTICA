#ifndef WORDS_H
#define WORDS_H

#include <Arduino.h>
#define START 1
#define RED_RGB_PIN 8
#define GREEN_RGB_PIN 9
#define BLUE_RGB_PIN 10

#define DIFFICULTY_BUTTON 2
#define START_BUTTON 3

bool buttonPressed = false;
bool buttonPressedStart = false;
int counter = 1;
unsigned long countdownStartTime = 0;
int countdownValue = 3; // Start from 3 seconds for the countdown
bool countdownActive = false;
unsigned long ultimaDebounceDifficulty = 0;     // Ultima dată când a fost schimbată starea butonului de dificultate
unsigned long ultimaDebounceStart = 0;     // Ultima dată când a fost schimbată starea butonului de start
unsigned long delayDebounce = 50;     // Timpul de debounce în milisecunde

bool stareUltimButon = HIGH;           // Starea precedentă a butonului
bool stareUltimButonStart = HIGH;

unsigned wordStartTime = 0;
unsigned int timeRound = 10000;
unsigned int timePerWord = 5000;
unsigned int userTime = 0;
int pointsCount = 0;

const char* cuvinteJoc[] = {
    "afara",
    "floare",
    "mare",
    "tare",
    "minunat",
    "malaka",
    "dalaka",

};

int freqWords[] = {0,0,0,0,0,0,0};

enum DIFICULTATE{
    EASY = START,
    MEDIUM,
    HARD
};

DIFICULTATE setDifficulty = EASY;

void changeDifficulty();
void startRound();
void execRound();
int generateWordIndex();
void stopRound();
void ledColor(int red, int blue, int green);

#endif
