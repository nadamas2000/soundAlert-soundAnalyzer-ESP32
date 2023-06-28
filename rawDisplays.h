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
  static unsigned short i = 0;
  static unsigned short ampPrev = 0;

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
  
  short peakMax = -MAX_READ_VALUE;
  short peakMin = MAX_READ_VALUE;
  chrono = micros(); // Sample window 10ms
  while (micros() - chrono < 10000ul) {
    short sample = analogRead(MIC_PIN) - SILENCE;
    peakMax = max(sample, peakMax);
    peakMin = min(sample, peakMin);
  }  

  short x = peakMax - peakMin;
  short amp = map(x, 0, MAX_READ_VALUE - SILENCE, 0, DISPLAY_HEIGHT - hOffset);
  amp = min(short(DISPLAY_HEIGHT - hOffset), amp);
  // Sweeping effect
  
  for (short j = 0; j < 7; j++) {
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
  static short data[DISPLAY_WIDTH] = {0};

  if (initial) {
    title[0] = "Running";
    title[1] = "Envelope";
    display.clearDisplay();
    display.setTextSize(1); 
    display.cp437(true);  // Use full 256 char 'Code Page 437' font
    // Clear data
    for (unsigned short i = 0; i < DISPLAY_WIDTH; i++) data[i] = 0;
  }
  
  hOffset = FONT_HEIGHT;
  short lostSound = data[0];
  for (unsigned short i = 0; i < DISPLAY_WIDTH - 1 ; i++) data[i] = data[i + 1]; // move data
  
  chrono = micros(); // Sample window 10ms
  while (micros() - chrono < 10000ul) {
    short sample = analogRead(MIC_PIN) - SILENCE;
    if (sample > ampMax) ampMax = sample;
    else if (sample < ampMin) ampMin = sample;
  }

  short x = ampMax - ampMin;
  data[DISPLAY_WIDTH - 1] = map(x, 0, short(MAX_READ_VALUE - SILENCE), 0, short(DISPLAY_HEIGHT - hOffset));
  data[DISPLAY_WIDTH - 1] = min(short(DISPLAY_HEIGHT - hOffset), data[DISPLAY_WIDTH - 1]);

  ampMin = DISPLAY_HEIGHT - hOffset;
  ampMax = 0;  
  display.drawLine (0, DISPLAY_HEIGHT - lostSound, 1, DISPLAY_HEIGHT - data[0], SSD1306_BLACK);
  for (short i = 1; i < DISPLAY_WIDTH; i++) {
    if (data[i] > ampMax) ampMax = data[i];
    if (data[i] < ampMin) ampMin = data[i];
    display.drawLine (i, DISPLAY_HEIGHT - (short)data[i - 1], i + 1, DISPLAY_HEIGHT - (short)data[i], SSD1306_BLACK);
    display.drawLine (i - 1, DISPLAY_HEIGHT - (short)data[i - 1], i, DISPLAY_HEIGHT - (short)data[i], SSD1306_WHITE);
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
  static const unsigned short SAMPLES = 128;  // = DISPLAY_WIDTH
  static short data[SAMPLES];
  static unsigned short midPoint_AMPB;

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
  for (unsigned short i = 0; i < DISPLAY_WIDTH; i++) {
    data[i] = analogRead(MIC_PIN);
    short amplitude = data[i] - SILENCE;    
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