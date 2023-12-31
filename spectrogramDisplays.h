/**
 * @file spectrogramDisplays.h
 * @brief Spectrogram Display Functions
 *
 * This file contains the functions used to display spectrograms on a display.
 *
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#pragma once

#include "board.h"
#include "display.h"

// Namespaces
#include "soundAnalysisToolsNamespaces.h"
using namespace commonSoundAnalysisTools;
using namespace commonDisplays;
using namespace commonSpectrum;

// Common Spectrogram variables
const unsigned short SAMPLES = 128; /**< Number of samples in the spectrogram */
const unsigned short N_COLORS = 2; /**< Number of colors in the spectrogram */
const unsigned short colors[N_COLORS] = {SSD1306_BLACK, SSD1306_WHITE}; /**< Colors used in the spectrogram */
unsigned short graphW; ///< Width of the graph
unsigned short wOffset; ///< Offset for width
int log2Sample = log(SAMPLES) / log(2); /**< Logarithm base 2 of the number of samples */
float _Complex data[SAMPLES]; /**< Data array for the spectrogram */

/**
 * @brief Prints a vertical line on the display.
 *
 * This function prints a vertical line on the display using the provided data array.
 * It iterates over the frequency data points and calculates the amplitude to determine
 * the color of each pixel in the line. The colors used are based on the provided array of colors.
 *
 * @param data Pointer to the data array.
 * @param nbFreqD Number of frequency data points.
 * @param x X-coordinate of the line.
 * @param nColors Number of colors.
 * @param colors Array of colors.
 */
void printVLine(float _Complex *data, int nbFreqD, int x, unsigned short nColors, const unsigned short *colors);

/**
 * @brief Displays the spectrogram.
 *
 * @details This function displays the spectrogram on the display. If the initial parameter is set to true, it clears the display
 * before displaying the spectrogram. The spectrogram is generated based on the provided parameters and data arrays.
 *
 * @param initial If true, clear the display before displaying the spectrogram.
 */
void displaySpectrogram(bool initial);

/**
 * @brief Displays the running spectrogram.
 *
 * @details This function displays the running spectrogram on the display. If the initial parameter is set to true, it clears the
 * display before displaying the running spectrogram. The running spectrogram is generated based on the provided
 * parameters and data arrays.
 *
 * @param initial If true, clear the display before displaying the running spectrogram.
 */
void displayRunningSpectrogram(bool initial);

/**
 * @brief Displays the sweeping spectrogram.
 *
 * @details This function displays the sweeping spectrogram on the display. If the initial parameter is set to true, it clears the
 * display before displaying the sweeping spectrogram. The sweeping spectrogram is generated based on the provided
 * parameters and data arrays.
 *
 * @param initial If true, clear the display before displaying the sweeping spectrogram.
 */
void displaySweepingSpectrogram(bool initial);

// Code
void printVLine(float _Complex *data, int nbFreqD, int x, unsigned short nColors, const unsigned short *colors) {
  for (int i = 1; i < nbFreqD; i++) {
    int amplitude = abs((int)creal(data[i]));
    unsigned short iColor = map(amplitude, 0, 160, 0, nColors - 1);
    if (iColor < 0) iColor = 0;
    if (iColor > nColors - 1) iColor = nColors - 1;
    short y = (DISPLAY_HEIGHT - hOffset) - i;
    display.writePixel(x, y, colors[iColor]);
  }
}

void displaySpectrogram(bool initial) {
  static long chronoTo1Sec; ///< Variable to track time for 1 second display
  static float nTimes = 70.0f; /**< Number of times to display the spectrogram */

  if (initial) {    
    title[0] = "1 Second";
    title[1] = "Spectrogram";    
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  
    display.setTextSize(1);
    display.clearDisplay();
    hOffset = FONT_HEIGHT - 1;
    wOffset = FONT_WIDTH;
    graphH = DISPLAY_HEIGHT - hOffset;
    graphW = DISPLAY_WIDTH - wOffset;

    // print vertical axis
    short k = 1;
    short vDist = (k * 16) + hOffset + 4;
    do {      
      display.setCursor(0, DISPLAY_HEIGHT - vDist);
      display.println(String(k * 2)); // 128 samples -> *2
      k++;
      vDist = (k * 16) + hOffset + 4;
    } while(vDist < DISPLAY_HEIGHT);

    // print horizontal axis
    unsigned char marks = 5;
    String nums[marks] = {"0", "0.25", "0.5", "0.75", "1"};
    for (unsigned char k = 0; k < marks; k++) {
      short mult = (DISPLAY_WIDTH - 1 - ((FONT_WIDTH * 2) + (nums[marks - 1].length() / 2))) / (marks - 1); // *2 by left and right margin, "1" by division margin error.
      short x = (6 + (k * mult)) - ((FONT_WIDTH * nums[k].length())/2);
      display.setCursor(x, graphH + 1);
      display.println(nums[k]);
    }
    display.setCursor(graphW, graphH + 1);
    display.println("s"); 
  }

  // Clear 2D graphic
  display.fillRect(wOffset, 0, DISPLAY_WIDTH, graphH, SSD1306_BLACK);
  float scaleW = (float)graphW / nTimes;
  bool printedVLines[graphW] = { false };
  for (int j = 0; j < (int)nTimes; j++) {
    if (digitalRead(BUTTON_P_PIN) == LOW) return;   // Exit in the middle of calcs.    
    getData(data, SAMPLES, log2Sample); 
    if (j == 0) printVLine(data, graphH, wOffset, N_COLORS, colors);
    else {
      short x = (j * scaleW);
      while (x > 0 && !printedVLines[x]) {  // Print all Graphic if nTimes < spectrogramGraphW.
        printVLine(data, graphH, x + wOffset, N_COLORS, colors);
        printedVLines[x] = true;
        x--;
      }    
    }
  } 
  
  // display vertical legend
  display.fillRect((FONT_WIDTH * 2) - 2, 0, (FONT_WIDTH * 3) + 2, FONT_HEIGHT + 1, SSD1306_BLACK);
  display.setCursor(FONT_WIDTH * 2, 0);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.println("kHz");
  
  // Autoadjust horizontal quality to show exactly 1 second.
  if (abs((long)(millis() - chronoTo1Sec - 1000ul)) > 1 ) {
    float modPercent = (float)1000ul / (float)(millis() - chronoTo1Sec);
    nTimes = nTimes * modPercent;;
  }
  chronoTo1Sec = millis();
}

void displayRunningSpectrogram(bool initial) {
  static unsigned short prevLines[DISPLAY_WIDTH][DISPLAY_HEIGHT]; /**< Previous lines in the running spectrogram */

  if (initial) {
    title[0] = "Running";
    title[1] = "Spectrogram";    
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  
    display.setTextSize(1);
    display.clearDisplay();
    wOffset = FONT_WIDTH;

    // clear prevLines
    for (short i = 0; i < DISPLAY_WIDTH; i++) {
      for (short j = 0; j < DISPLAY_HEIGHT; j++) {
        prevLines[i][j] = 0;
      }
    }
    
    // print vertical axis    
    short k = 1;
    short vDist = (k * 16) + 4;
    do {      
      display.setCursor(0, (DISPLAY_HEIGHT + 1) - vDist);
      display.println(String(k * 2)); // 128 samples -> *2
      k++;
      vDist = (k * 16) + 4;
    } while(vDist < DISPLAY_HEIGHT);
  }  

  getData(data, SAMPLES, log2Sample);

  for (unsigned short i = 1; i <= DISPLAY_HEIGHT; i++) {
    int amplitude = abs((int)creal(data[i+1]));
    unsigned short iColor = map(amplitude, 0, 160, 0, N_COLORS - 1);
    if (iColor < 0) iColor = 0;
    if (iColor > N_COLORS - 1) iColor = N_COLORS - 1;
    short y = DISPLAY_HEIGHT - i;
    display.writePixel(wOffset, y, colors[iColor]);
    prevLines[wOffset][y] = colors[iColor];
  }

  // Print and move previous graphics.
  for (short i = DISPLAY_WIDTH - 2; i > wOffset; i--) {
    for (short j = 0; j < DISPLAY_HEIGHT; j++) {
      prevLines[i][j] = prevLines[i - 1][j];      
      display.writePixel(i, j, prevLines[i][j]);
    }
  }
  
  // display vertical legend
  display.fillRect(0, 0, (FONT_WIDTH * 3) + 2, FONT_HEIGHT, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.println("kHz");
}

void displaySweepingSpectrogram(bool initial) {
  static unsigned short xPos = 0; /**< X position of the sweeping spectrogram */

  if (initial) {
    title[0] = "Sweeping";
    title[1] = "Spectrogram";  
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  
    display.setTextSize(1);
    display.clearDisplay();
    wOffset = FONT_WIDTH;
    graphW = display.width() - wOffset;
    xPos = 0;
    
    // print vertical axis    
    short k = 1;
    short vDist = (k * 16) + 4;
    do {      
      display.setCursor(0, (DISPLAY_HEIGHT + 1) - vDist);
      display.println(String(k * 2)); // 128 samples -> *2
      k++;
      vDist = (k * 16) + 4;
    } while(vDist < DISPLAY_HEIGHT);
  }

  getData(data, SAMPLES, log2Sample);
  
  // Sweeping effect
  display.drawFastVLine(xPos + wOffset + 1, 0, DISPLAY_HEIGHT, SSD1306_WHITE);
  display.drawFastVLine(xPos + wOffset + 2, 0, DISPLAY_HEIGHT, SSD1306_BLACK);
  display.drawFastVLine(xPos + wOffset + 3, 0, DISPLAY_HEIGHT, SSD1306_BLACK);
  display.drawFastVLine(xPos + wOffset + 4, 0, DISPLAY_HEIGHT, SSD1306_BLACK);

  // Draw data
  for (unsigned short i = 1; i <= DISPLAY_HEIGHT; i++) {
    int amplitude = abs((int)creal(data[i + 1]));
    unsigned short iColor = map(amplitude, 0, 160, 0, N_COLORS - 1);
    if (iColor < 0) iColor = 0;
    if (iColor > N_COLORS - 1) iColor = N_COLORS - 1;
    short y = DISPLAY_HEIGHT - i;
    display.writePixel(xPos + wOffset, y, colors[iColor]);
  }
  xPos = (xPos + 1) % graphW;

  // display vertical legend
  display.fillRect(0, 0, (FONT_WIDTH * 3) + 2, FONT_HEIGHT + 1, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.println("kHz");
}