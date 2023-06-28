/**
 * @file spectrumDisplays.h
 * @brief This file contains the code for displaying sound spectrum.
 *
 * This file includes the necessary functions and variables to display
 * sound spectrum on a display. It provides two different display modes:
 * "Vertical Lines" and "Continuous Line". The spectrum data is
 * acquired using Fast Fourier Transform (FFT) algorithms and is
 * processed to generate the graphical representation on the display.
 * The display can show either vertical lines or a continuous line graph
 * representing the frequency and amplitude of the sound spectrum.
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
using namespace minMax;

// Headers
/**
 * @brief Prints the spectrum display with continuous lines.
 *
 * @details This function prints the spectrum display with continuous lines using the data array.
 * It calculates the amplitude of each frequency bin and draws a line from the previous bin's amplitude
 * to the current bin's amplitude. It also calculates and displays the maximum amplitude and frequency.
 *
 * @param SAMPLES The number of samples in the data array.
 * @param data The array containing the spectrum data.
 * @param peak The array storing the peak values.
 */
void printSpectrumContinuousLineGraphic(unsigned short SAMPLES, float _Complex *data, unsigned char *peak);

/**
 * @brief Prints the spectrum display with vertical lines.
 *
 * @details This function prints the spectrum display with vertical lines using the data array.
 * It calculates the amplitude of each frequency bin and draws a vertical line representing the amplitude.
 * It also calculates and displays the maximum amplitude and frequency.
 *
 * @param SAMPLES The number of samples in the data array.
 * @param data The array containing the spectrum data.
 * @param peak The array storing the peak values.
 */
void printSpectrumVLinesGraphic(unsigned short SAMPLES, float _Complex *data, unsigned char *peak);

/**
 * @brief Displays the spectrum.
 *
 * This function displays the spectrum using the specified display mode.
 * If the mode is set to 0, it displays the spectrum with vertical lines.
 * If the mode is set to 1, it displays the spectrum with continuous lines.
 * The function acquires the sound data, applies a window function, and performs
 * a Fast Fourier Transform (FFT) to obtain the spectrum data. It then calls
 * the appropriate function to print the spectrum display based on the mode.
 *
 * @param initial Specifies if it's the initial display.
 * @param mode The display mode (0 for vertical lines, 1 for continuous line).
 */
void displaySpectrum(bool initial, unsigned char mode);

/**
 * @brief Displays the spectrum bars.
 *
 * This function displays the spectrum using bars. It acquires the sound data,
 * applies a window function and performs a Fast Fourier Transform (FFT) to obtain
 * the spectrum data. The spectrum is then visualized using vertical bars, where the
 * height of each bar represents the amplitude of a frequency bin. The number of bars
 * displayed is determined by the number of samples and frequency resolution. The
 * function also displays the corresponding frequency values on the x-axis.
 *
 * @param initial Specifies if it's the initial display.
 * */
void displaySpectrumBars(bool initial);
  

// Code
void printSpectrumContinuousLineGraphic(unsigned short SAMPLES, float _Complex *data, unsigned char *peak) {
  static long chronoInfo;
  static int freqMaxInfo;
  static int ampMaxInfo;
  const int lowFilterInfo = 800;  

  int nFreq = SAMPLES / 2;
  hOffset = FONT_HEIGHT;
  graphH = DISPLAY_HEIGHT - hOffset;  
  ampMax = 0;
  unsigned short imax = 0;
  unsigned char prevPeak = -1;
  int peakInterval = 3; // recomends odd number;
  for (unsigned short i = 1; i < min(nFreq, DISPLAY_WIDTH); i++) {    
    double currentAmplitude = creal(data[i]);
    double nextAmplitude = creal(data[i + 1]);
    if (i == DISPLAY_WIDTH - 1) nextAmplitude == currentAmplitude;    
    if (currentAmplitude > ampMax) {
      ampMax = (int)currentAmplitude;
      imax = i;
    }
    short cAmp = map(currentAmplitude, 0.0, (double)MAX_READ_VALUE * 2, 0.0, (double)graphH);
    short nAmp = map(nextAmplitude, 0.0, (double)MAX_READ_VALUE * 2, 0.0, (double)graphH);
    cAmp = max(short(0), cAmp);
    nAmp = max(short(0), nAmp);
    display.drawLine(i - 1, graphH - cAmp, i, graphH - nAmp, SSD1306_WHITE);

    // Print peak    
    if (i % peakInterval == (peakInterval / 2)) {
      int maxPeak = 0;
      for (int j = 0; j < peakInterval; j++) {
        int index = i + j - (peakInterval / 2);
        if (index > 1 && index < min(nFreq, DISPLAY_WIDTH)){
          maxPeak = max(maxPeak, (int)creal(data[index]));
        }        
      }      
      maxPeak = map(maxPeak, 0, MAX_READ_VALUE * 2, 0, graphH);
      peak[i] = max((unsigned char)maxPeak, peak[i]);
      peak[i] = min(peak[i], (unsigned char)graphH);
      if (prevPeak >= 0 && i > 1) {
        display.drawLine(i - peakInterval, graphH - prevPeak, i, graphH - peak[i], SSD1306_WHITE);
      }
      peak[i] = max(0, peak[i] - 1);
      prevPeak = peak[i];      
    }   
  }  
    
  if (millis() - chronoInfo >= 1000ul) ampMaxInfo = 0;
  if (ampMax > ampMaxInfo & ampMax > lowFilterInfo) {
    freqMaxInfo = imax * 15.2256 * (1024.0 / SAMPLES);
    ampMaxInfo = ampMax;
    chronoInfo = millis();
  }
  
  display.setTextSize(1);
  if (ampMaxInfo > 0) {
    char texte[14];
    sprintf(texte, "%4d Hz", freqMaxInfo);
    display.setCursor(DISPLAY_WIDTH - 43, 0);
    display.println(texte);
    
    sprintf(texte, "%5d Am", ampMaxInfo);
    display.setCursor(DISPLAY_WIDTH - 49, FONT_HEIGHT);
    display.println(texte);
  } else {
    display.setCursor(DISPLAY_WIDTH - 25, 0);
    display.println("- Hz");
    display.setCursor(DISPLAY_WIDTH - 25, FONT_HEIGHT);
    display.println("- Am");
  } 
}

void printSpectrumVLinesGraphic(unsigned short SAMPLES, float _Complex *data, unsigned char *peak) {
  static long chronoInfo;
  static int freqMaxInfo;
  static int ampMaxInfo;
  const short lowFilterInfo = 800;  

  int nFreq = SAMPLES / 2;
  hOffset = FONT_HEIGHT;
  graphH = DISPLAY_HEIGHT - hOffset; 
  ampMax = 0;
  unsigned short imax = 0;
  for (unsigned short i = 1; i < min(nFreq, (int)DISPLAY_WIDTH); i++) {
    // Extract amplitude and max. 
    int amplitude = max(0, (int)creal(data[i]));
    if (amplitude > ampMax) {
      ampMax = amplitude;
      imax = i;
    }

    // Print vertical line
    short reducedAmplitude = map(amplitude, 0, MAX_READ_VALUE * 2, 0, graphH);
    display.drawFastVLine(i - 1, graphH - reducedAmplitude, reducedAmplitude, SSD1306_WHITE);
    
    // Print peak
    if (i % 3 == 1) {
      int maxPeak = max(0, (int)creal(data[i]));
      if (i > 1) maxPeak = max(maxPeak, max(0, (int)creal(data[i - 1])));
      if (i < DISPLAY_WIDTH - 1) maxPeak = max(maxPeak, max(0, (int)creal(data[i + 1])));      
      maxPeak = map(maxPeak, 0, MAX_READ_VALUE * 2, 0, graphH);
      peak[i] = max((unsigned char)maxPeak, peak[i]);
      peak[i] = min(peak[i], (unsigned char)graphH);
      if (peak[i] > 8) {
        short y = graphH - peak[i];
        display.drawLine(i - 1, y, i + 1, y, SSD1306_WHITE);
        peak[i] -= 1;
      }
    }    
  }  
  
  // Calc. freq. and info
  if (millis() - chronoInfo >= 1000ul) ampMaxInfo = 0;
  if (ampMax > ampMaxInfo & ampMax > lowFilterInfo) {
    freqMaxInfo = imax * 15.2256 * (1024.0 / SAMPLES); // 985.0? * 15.2256 * (1024.0 / LISTEN_SAMPLES)
    ampMaxInfo = ampMax;
    chronoInfo = millis();
  }
  
  // Print info
  display.setTextSize(1);
  if (ampMaxInfo > 0) {
    char texte[14];
    sprintf(texte, "%4d Hz", freqMaxInfo);
    display.setCursor(DISPLAY_WIDTH - 43, 0);
    display.println(texte);
    
    sprintf(texte, "%5d Am", ampMaxInfo);
    display.setCursor(DISPLAY_WIDTH - 49, FONT_HEIGHT);
    display.println(texte);
  } else {
    display.setCursor(DISPLAY_WIDTH - 25, 0);
    display.println("- Hz");
    display.setCursor(DISPLAY_WIDTH - 25, FONT_HEIGHT);
    display.println("- Am");
  } 
}

void displaySpectrum(bool initial, unsigned char mode) {
  static const unsigned short SAMPLES = 256;
  static const int log2Sample = log(SAMPLES) / log(2); 
  static float _Complex data[SAMPLES];
  static unsigned char peak[SAMPLES] = {0};

  if (mode == 0) {
    title[0] = "Spectrum";
    title[1] = "Vertical Lines";
  }
  else if (mode == 1) {
    title[0] = "Spectrum";
    title[1] = "Continuous Line";
  }

  getData(data, SAMPLES, log2Sample);

  int nFreq = SAMPLES / 2;
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);  
  display.setTextSize(1);
  float freqs[6] = {1, 2, 3, 4, 5, 6};
  for (unsigned char i = 0; i < 6; i++) {
    String numTxt = String((int)freqs[i]);
    short pos = (16 * freqs[i]) - ((FONT_WIDTH * numTxt.length()) / 2);
    display.setCursor(pos, DISPLAY_HEIGHT - FONT_HEIGHT + 2);    
    display.println(numTxt);
  }
  String kHz = "kHz";
  display.setCursor(DISPLAY_WIDTH - (FONT_WIDTH * kHz.length()), DISPLAY_HEIGHT - FONT_HEIGHT + 2);
  display.println(kHz);

  if (mode == 0) printSpectrumVLinesGraphic(SAMPLES, data, peak);
  else if (mode == 1) printSpectrumContinuousLineGraphic(SAMPLES, data, peak);
}

void displaySpectrumBars(bool initial) {
  static const unsigned short SAMPLES = 128;
  static const int log2Sample = log(SAMPLES) / log(2);
  static float _Complex data[SAMPLES];

  if (initial) {
    title[0] = "Spectrum Bars";
    hOffset = FONT_HEIGHT + 1;
    graphH = DISPLAY_HEIGHT - hOffset;
  }  

  getData(data, SAMPLES, log2Sample);

  int nFreq = SAMPLES / 2;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  const float freqs[4] = {1, 2, 3, 4};
  for (unsigned char i = 0; i < 4; i++) {
    String numTxt = String((int)freqs[i] * 2);    
    short pos = (31 * freqs[i]) - ((FONT_WIDTH * numTxt.length()) / 2);
    display.setCursor(pos, graphH + 1);
    display.println(numTxt);
  }

  display.setCursor(0, graphH + 1);
  display.println("kHz");  
  
  for (short i = 0; i < 16; i++) {
    short amplitude = 0;
    for (short j = 0; j < 4; j++) {
      int sample = (int)creal(data[i * 4 + j + 2]);
      short candidate = map(sample, 0, MAX_READ_VALUE, 0, graphH);
      if (amplitude < candidate) amplitude = candidate;
    }
    display.fillRect(i * 8, DISPLAY_HEIGHT - hOffset - amplitude, 6, amplitude, SSD1306_WHITE);
  }
}