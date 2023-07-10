/**
 * @file soundInfo.h
 * @brief This file contains functions for sound analysis and displaying sound information.
 * 
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#pragma once

#include "fft.h"
#include "listenLogic.h"
#include "display.h"
#include "pair.h"

// -------------- Listening global variables and constants ------------------
const int LISTEN_SAMPLES = 1024;
int maxCounter[LISTEN_SAMPLES] = {0};
int bestThree[3] = {0,0,0};

// ---------------- Headers ----------------------
/**
 * @brief Displays the relevant information for the listening mode on the display.
 * @param vOffset The vertical offset for displaying the information.
 * @param maxA The maximum amplitude value.
 * @param maxI The index corresponding to the maximum amplitude.
 */
void showListeningInfo(short vOffset);

/**
* @brief Reads the sound data from the microphone.
* @param data The array to store the sound data.
*/
void getSound(float _Complex *data);

/**
 * @brief Analyzes the sound data using Fast Fourier Transform (FFT).
 * @return A Pair object containing the maximum amplitude and its corresponding index.
 */
Pair<float, int> analyzeSound();

/**
 * @brief Extracts the relevant information from the analyzed sound data.
 * @param data The analyzed sound data.
 * @param maxA Reference to store the maximum amplitude.
 * @param maxI Reference to store the index corresponding to the maximum amplitude.
 */
void getRellevantInfo(float _Complex *data, float &maxA, int &maxI);

/**
 * @brief Displays the sound information on the display.
 */
void displaySoundInfo();


// ---------------- Code ----------------------
void showListeningInfo(short vOffset, float maxA, int maxI) {  
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(0, vOffset + 0);
  display.println("Mark: " + String(maxI));
  display.setCursor(0, vOffset + FONT_HEIGHT);
  display.println("AHz: " + String(maxA));
  display.setCursor(0, vOffset + FONT_HEIGHT * 2);
  display.println("Hz: " + String(maxI * 15.2256 * (1024.0 / LISTEN_SAMPLES)));
  display.setCursor(0, vOffset + FONT_HEIGHT * 3);  
  
  if (maxA > 20000) {
    int i = 30;
    while (i < LISTEN_SAMPLES) {    
      if (maxCounter[i] > maxCounter[bestThree[0]]) {
        bestThree[2] = bestThree[1];
        bestThree[1] = bestThree[0];
        bestThree[0] = i;
      } else {
        if (maxCounter[i] > maxCounter[bestThree[1]] and bestThree[0] != i) {
          bestThree[2] = bestThree[1];        
          bestThree[1] = i;
        } else if (maxCounter[i] > maxCounter[bestThree[2]] and bestThree[0] != i and bestThree[1] != i) bestThree[2] = i;
      }    
      i++;
      while (maxCounter[i] <= maxCounter[bestThree[2]] and i < LISTEN_SAMPLES) i++;
    }
  }
  
  
  String txt = "";
  for (int i = 0; i < 3; ++i) {
    if (i > 0 and bestThree[i] > 0) txt += ", ";
    if (bestThree[i] > 0) txt += String(bestThree[i]);     
  }
  display.setCursor(0, vOffset + FONT_HEIGHT * 3);
  display.println("B3: " + txt);
  display.display();
  
}


// ---------------- Sound analyze ----------------------
void getSound(float _Complex *data) {
  static long chrono;
  static const int LISTEN_MAX_FREQ = 16; // kHz
  static const unsigned long sampling_period_us = round(1000ul * (1.0 / LISTEN_MAX_FREQ)); // 1/Hz = T(s) -> 1/kHz = T(ms)
  
  for (int i = 0; i < LISTEN_SAMPLES; i++) {
    chrono = micros();
    data[i] = analogRead(MIC_PIN);    
    while (micros() - chrono < sampling_period_us); // only if analogRead time < sampling_period_us
  }
}

void getRellevantInfo(float _Complex *data, float &maxA, int &maxI) {
  maxA = 0;
  maxI = 0;
  for (int i = 1; i < LISTEN_SAMPLES; i++) {
    if (creal(data[i]) > maxA) {
      maxA = creal(data[i]);
      maxI = i;    
    }
  }
  maxCounter[maxI]++;
  
}

Pair<float, int> analyzeSound() {
  static float _Complex data[LISTEN_SAMPLES];
  static const int log2Sample = log(LISTEN_SAMPLES) / log(2);
  static float maxA = 0;
  static int maxI = 0;
  
  getSound(data);   
  applyWindow (data, log2Sample, HAMMING, FFT_FORWARD);
  performFFT(data, log2Sample, FFT_FORWARD);
  getRellevantInfo(data, maxA, maxI);

  Pair<float, int> max = {maxA, maxI};
  return max;
}


void displaySoundInfo() {
  Pair<float, int> maxVal = analyzeSound();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  String txt = "Sound Info";
  display.setCursor((DISPLAY_WIDTH - (txt.length() * FONT_WIDTH)) / 2, 0);
  display.println("Sound Info");  
  showListeningInfo(FONT_HEIGHT * 2, maxVal.first, maxVal.second);
}