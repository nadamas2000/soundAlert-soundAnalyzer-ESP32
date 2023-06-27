/**
 * @file soundAnalysisTools.h
 * @brief Sound Analysis Tools
 *
 * This file contains the implementation of sound analysis tools that
 * display various visualizations of sound data on a display.
 *
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#include "board.h"
#include "display.h"

// Display libraries
#include "rawDisplays.h" // Analysis display modes
#include "spectrumDisplays.h"
#include "spectrogramDisplays.h"

// Namespaces
#include "soundAnalysisToolsNamespaces.h"
using namespace commonSoundAnalysisTools;
using namespace minMax;

// Globals
const uint8_t MAXMODES = 9;
int8_t mode = 0;
bool changeMode = false;
bool toolSection = false;
unsigned long displayTitle[MAXMODES];


/**
 * @brief Initialize the sound analysis tools.
 *
 * @details This function initializes the variables used by the sound analysis tools.
 * It sets default values for various parameters and flags.
 */
void initSoundAnalysisTools();

/**
 * @brief Show the title on the display.
 *
 * @details This function displays the title of the current mode on the display for a
 * specific duration. After the duration has elapsed, the title is cleared
 * from the display.
 *
 * @param mode The current display mode.
 */
void showTitle(uint8_t mode);

/**
 * @brief Print the title lines on the display.
 *
 * @details This function prints the title lines on the display. The title lines are
 * centered vertically and horizontally on the display.
 *
 * @param black Set to true to print the title lines in black color.
 *              Set to false to print the title lines in white color.
 */
void printTitle(bool black);

/**
 * @brief Check the physical button action.
 *
 * @details This function checks the state of the physical button and performs actions
 * based on the button press. If the button is pressed, it changes the display mode.
 */
void checkButton();

/**
 * @brief Select the display mode.
 *
 * @details This function selects the appropriate display mode based on the current mode
 * value. It calls the corresponding display function for the selected mode.
 */
void selectDisplayMode();



void initSoundAnalysisTools() {
  for (uint8_t i = 0; i < MAXMODES; i++) {
    displayTitle[i] = 0;
  }

  displayTitle[mode] = millis();
}

void showTitle(uint8_t mode) {
  static unsigned long timeDisplayTitle = 2000ul;

  if (displayTitle[mode] > 0ul) {
    if ((millis() - displayTitle[mode]) > timeDisplayTitle) {
      displayTitle[mode] = 0ul;
    } 
  } 
}

void printTitle(bool black) {
  // Default Adafruit font 9x5 pixels.
  int nTitles = 0;
  for (int i = 0; i < 7; i++) if (title[i].length() > 0) nTitles++;
  int y = (display.height() / 2) - ((FONT_HEIGHT * nTitles) / 2);
  for (int i = 0; i < nTitles; ++i) {
    int x = (display.width() / 2) - ((FONT_WIDTH * title[i].length()) / 2);
    display.setCursor(x, y + (i * FONT_HEIGHT)) ;
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    if (black) display.setTextColor(SSD1306_BLACK, SSD1306_BLACK);
    display.println(title[i]);    
  }
}

void checkButton() {
  static unsigned long chronoButton = millis();
  static bool buttonStatus = digitalRead(BUTTON_P_PIN);

  changeMode = false;
  // Push the button to change modes
  if (millis() - chronoButton > 500ul) {    
    if (buttonStatus != digitalRead(BUTTON_P_PIN) && buttonStatus == LOW) { // Modes up
      delay(30);
      mode = (mode + 1) % MAXMODES;
      changeMode = true;
      chronoButton = millis();
      awakeDuration = (MINUTES_AWAKE * 60 * 1000);
      lastActivity = millis();
      toolSection = true;

      // Clear title on change
      for (uint8_t i = 0; i < 7; i++) title[i] = "";
      displayTitle[mode] = millis();
    }
    buttonStatus = digitalRead(BUTTON_P_PIN);
  }
}

void selectDisplayMode() {
  static bool prevShowTitle = true;

  showTitle(mode);
  switch (mode) {
    case 0: // Display spectrum 
      displaySpectrum(changeMode, 0);
      break;
    case 1: // Display spectrum 
      displaySpectrum(changeMode, 1);
      break;
    case 2: // Display spectrum bars        
      displaySpectrumBars(changeMode);
      break;
    case 3: // Display Amplitude bars
      displayAmplitudeBars(changeMode);
      break;
    case 4: // Display Sweeping Envelope
      displaySweepingEnvelope(changeMode);
      break;
    case 5: // Display Running Envelope
      displayRunningEnvelope(changeMode);
      break;
    case 6: // Display 1s Spectrogram
      displaySpectrogram(changeMode);
      break;
    case 7: // Display Sweeping Spectrogram
      displaySweepingSpectrogram(changeMode);      
      break;
    case 8: // Display Running Spectrogram
      displayRunningSpectrogram(changeMode);
      break;
    default:
      break;
  }

  if (displayTitle[mode] > 0ul) {
    printTitle(false);
    prevShowTitle = true;
  } else if (prevShowTitle) {
    prevShowTitle = false;
    printTitle(true);
  }

  display.display();
}