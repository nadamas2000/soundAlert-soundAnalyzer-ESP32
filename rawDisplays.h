/**
 * @file rawDisplays.h
 * @brief Display Functions for Sound Analysis Tools
 *
 * This file contains the implementation of various display functions for sound analysis tools.
 * The functions display the sweeping envelope graph, running envelope graph, and amplitude bars graph.
 *
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#include "board.h"
#include "display.h"

// Namespaces
#include "soundAnalysisToolsNamespaces.h"
using namespace commonSoundAnalysisTools;
using namespace commonDisplays;
using namespace minMax;

// Running Envelope global variables
int16_t sound_RENV[DISPLAY_WIDTH] = {0};

// Amplitude bars global variables
#define AMPLITUDE_BARS_SAMPLES 128  // = DISPLAY_WIDTH
int16_t sound_AMPB[AMPLITUDE_BARS_SAMPLES];
uint16_t midPoint_AMPB;

// Headers
/**
 * @brief Display the sweeping envelope graph.
 *
 * @details This function displays the sweeping envelope graph on the display. It
 * samples the sound input from the microphone and calculates the peak values
 * to determine the amplitude of the sound. The graph is updated in real-time,
 * creating a sweeping effect as the values change. The graph is displayed
 * vertically, with higher amplitudes shown at the bottom and lower amplitudes
 * at the top.
 * 
 * @param initial Set to true to clear the display and draw the fixed content.
 *              Recommended to use only the first time the function is called.
 */
void displaySweepingEnvelope(bool initial);

/**
 * @brief Display the running envelope graph.
 *
 * @details This function displays the running envelope graph on the display. It samples
 * the sound input from the microphone and calculates the peak values to
 * determine the amplitude of the sound. The graph shows the envelope of the
 * sound over time, with the oldest data gradually fading out as new data is
 * added. The graph is displayed vertically, with higher amplitudes shown at
 * the bottom and lower amplitudes at the top.
 *  
 * @param initial Set to true to clear the display and draw the fixed content.
 *              Recommended to use only the first time the function is called.
 */
void displayRunningEnvelope(bool initial);

/**
 * @brief Display the amplitude bars graph.
 *
 * @details This function displays the amplitude bars graph on the display. It samples
 * the sound input from the microphone and calculates the amplitudes of the
 * sound. The graph consists of vertical bars, where each bar represents the
 * amplitude of a specific sample. The height of each bar corresponds to the
 * amplitude value, with higher and lower amplitudes shown as bars.
 *  
 * @param initial Set to true to initialize the display and set up the fixed content.
 *                Recommended to use only the first time the function is called.
 */
void displayAmplitudeBars(bool initial);

// Code
void displaySweepingEnvelope(bool initial) { 
  static uint16_t i = 0;
  static uint16_t ampPrev = 0;

  if (initial) {
    title[0] = "Sweeping";
    title[1] = "Envelope";
    display.clearDisplay();
    display.setTextSize(1); 
    display.cp437(true);  // Use full 256 char 'Code Page 437' font
    hOffset = FONT_HEIGHT;
    i = 0;
  }  
   
  if (i >= DISPLAY_WIDTH || i == 0) {
    i = 0;
    ampMax = -MAX_READ_VALUE;
    ampMin = MAX_READ_VALUE;
  }  
  
  int16_t peakMax = -MAX_READ_VALUE;
  int16_t peakMin = MAX_READ_VALUE;
  chrono = micros(); // Sample window 10ms
  while (micros() - chrono < 10000ul) {
    int16_t sample = analogRead(MIC_PIN) - SILENCE;
    peakMax = max(sample, peakMax);
    peakMin = min(sample, peakMin);
  }  

  int16_t x = peakMax - peakMin;
  int16_t amp = map(x, 0, MAX_READ_VALUE - SILENCE, 0, DISPLAY_HEIGHT - hOffset);
  amp = min(int16_t(DISPLAY_HEIGHT - hOffset), amp);
  // Sweeping effect
  
  for (uint16_t j = 0; j < 7; j++) {
    int color = SSD1306_BLACK;
    if (j == 1) color = SSD1306_WHITE;
    display.drawFastVLine((i + j) % DISPLAY_WIDTH, hOffset, DISPLAY_HEIGHT, color);
  }
  display.drawLine (i - 1, DISPLAY_HEIGHT - ampPrev, i, DISPLAY_HEIGHT - amp, SSD1306_WHITE);
  ampPrev = amp;    

  bool changeMaxMin = false;
  if (peakMax > ampMax) {
    ampMax = peakMax;
    changeMaxMin = true;
  }
  if (peakMin > ampMin) {
    ampMin = peakMin;
    changeMaxMin = true;
  }
 
  if (i == 0 || changeMaxMin) {
    if (amp > ampMax) ampMax = amp;
    if (amp < ampMin) ampMin = amp;
    display.fillRect(0, 0, DISPLAY_WIDTH, FONT_HEIGHT, SSD1306_BLACK);    
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    String text = "Min: " + String(ampMin);
    display.println(text);
    display.setCursor(FONT_WIDTH * 10, 0);  
    text = "Max: " + String(ampMax);
    display.println(text);
  }
  ++i;
}

void displayRunningEnvelope(bool initial) {  
  if (initial) {
    title[0] = "Running";
    title[1] = "Envelope";
    display.clearDisplay();
    display.setTextSize(1); 
    display.cp437(true);  // Use full 256 char 'Code Page 437' font
    // Clear data
    for (uint16_t i = 0; i < DISPLAY_WIDTH; i++) sound_RENV[i] = 0;
  }
  
  hOffset = FONT_HEIGHT;
  int16_t lostSound = sound_RENV[0];
  for (uint16_t i = 0; i < DISPLAY_WIDTH - 1 ; i++) sound_RENV[i] = sound_RENV[i + 1]; // move data
  
  chrono = micros(); // Sample window 10ms
  while (micros() - chrono < 10000ul) {
    int16_t sample = analogRead(MIC_PIN) - SILENCE;
    if (sample > ampMax) ampMax = sample;
    else if (sample < ampMin) ampMin = sample;
  }

  int16_t x = ampMax - ampMin;
  sound_RENV[DISPLAY_WIDTH - 1] = map(x, 0, int16_t(MAX_READ_VALUE - SILENCE), 0, int16_t(DISPLAY_HEIGHT - hOffset));
  sound_RENV[DISPLAY_WIDTH - 1] = min(int16_t(DISPLAY_HEIGHT - hOffset), sound_RENV[DISPLAY_WIDTH - 1]);

  ampMin = DISPLAY_HEIGHT - hOffset;
  ampMax = 0;  
  display.drawLine (0, DISPLAY_HEIGHT - lostSound, 1, DISPLAY_HEIGHT - sound_RENV[0], SSD1306_BLACK);
  for (int16_t i = 1; i < DISPLAY_WIDTH; i++) {
    if (sound_RENV[i] > ampMax) ampMax = sound_RENV[i];
    if (sound_RENV[i] < ampMin) ampMin = sound_RENV[i];
    display.drawLine (i, DISPLAY_HEIGHT - (int16_t)sound_RENV[i - 1], i + 1, DISPLAY_HEIGHT - (int16_t)sound_RENV[i], SSD1306_BLACK);
    display.drawLine (i - 1, DISPLAY_HEIGHT - (int16_t)sound_RENV[i - 1], i, DISPLAY_HEIGHT - (int16_t)sound_RENV[i], SSD1306_WHITE);
  }  

  display.fillRect(0, 0, DISPLAY_WIDTH, FONT_HEIGHT, SSD1306_BLACK);  
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  String text = "Min: " + String(ampMin);
  display.println(text);
  display.setCursor(FONT_WIDTH * 10, 0);  
  text = "Max: " + String(ampMax);
  display.println(text);
}

void displayAmplitudeBars(bool initial) {  
  if (initial) {
    title[0] = "Amplitude Bars";
    hOffset = FONT_HEIGHT - 1;
    graphH = DISPLAY_HEIGHT - hOffset;
    midPoint_AMPB = (graphH / 2) + hOffset;    
    display.setTextSize(1); 
    display.cp437(true);  // Use full 256 char 'Code Page 437' font
  }
  ampMax = 0;
  ampMin = MAX_READ_VALUE;
  
  display.clearDisplay();
  for (uint16_t i = 0; i < DISPLAY_WIDTH; i++) {
    sound_AMPB[i] = analogRead(MIC_PIN);
    int16_t amplitude = sound_AMPB[i] - SILENCE;    
    ampMax = max(ampMax, amplitude);
    ampMin = min(ampMin, amplitude);
    amplitude *= (float)graphH / (float)MAX_READ_VALUE;
    display.drawLine(i, midPoint_AMPB, i, midPoint_AMPB - amplitude, SSD1306_WHITE);    
  } 

  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.println("Max: " + String(ampMax));
  display.setCursor(60, 0);
  display.println("Min: " + String(ampMin));
}