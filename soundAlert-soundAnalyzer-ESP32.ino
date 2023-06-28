/******************************************************************************************
 * @file SoundAnalyzerWifiKit32.ino
 * @brief Sound Alert System
 *
 * Program: Sound Alert System
 * Description: This program utilizes Arduino and various libraries to create a sound alert
 * system. The main functionality of the code is to detect specific frequencies with
 * certain intensities and display corresponding alerts on an OLED display.
 * 
 * Hardware:
 * - Microcontroller Board: Heltec Wifi Kit 32 V3
 * - Microphone: Max9814
 * - Battery Holder: AAA Battery Holder
 * - Case: 3D printed case
 *
 * The program includes different sections for hardware configuration, display
 * initialization, sound analysis tools, and the main loop functionality. It utilizes the
 * Arduino IDE and libraries such as SPI, Wire, FFT algorithms, and the SSD1306 OLED display
 * library.
 *
 * The system continuously captures sound through the Max9814 microphone, performs fast
 * Fourier transform (FFT) analysis on the sound data, and checks for specific frequencies
 * and intensity thresholds. When a detection occurs, an alert is triggered, and the
 * corresponding message is displayed on the OLED screen.
 *
 * Note: The complete code includes additional files and libraries that are not shown in
 * this snippet.
 *
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 *
 * References:
 * - Heltec Wifi Kit 32 V3 board: https://heltec.org/project/wifi-kit-32-v3/
 * - Microphone Max9814: https://www.analog.com/media/en/technical-documentation/data-sheets/max9814.pdf
 * - Adafruit SSD1306 Library: https://github.com/adafruit/Adafruit_SSD1306/
 * - Arduino Libraries: https://www.arduino.cc/reference/en/libraries/
 ******************************************************************************************/

#include "Arduino.h"
#include "board.h"
#include "display.h"

// General use globals
bool debug = false;

// Alerts and times globals
const int SECONDS_LISTENING_ON_WAKE_UP = 2;
const int MINUTES_AWAKE = 2;
int awakeDuration = SECONDS_LISTENING_ON_WAKE_UP * 1000;
unsigned long lastActivity;
#include "listenLogic.h"

// Tools section: Tools for sound analysis. Press 'P' and release the button when the microphone icon is displayed to enter and switch between modes.
#include "soundAnalysisTools.h" // Analysis mode selection and title display


/**
 * Prints a low battery alert on the OLED display.
 */
void printLowBatteryAlert();

/**
 * Handles the activity logic of the program, including button presses, sound analysis,
 * and entering sleep mode.
 */
void activityLogic();

/**
 * Sets up the initial configuration of the program. This includes board initialization,
 * display initialization, variable initialization, and verifying the wake-up reason.
 */
void setup();

/**
 * The main loop of the program. It handles the activity logic based on the battery status
 * and triggers low battery alerts when necessary.
 */
void loop();

// Code
void printLowBatteryAlert() {
  // Low battery alert
  display.clearDisplay();
  delay(500);
  int16_t x = (DISPLAY_WIDTH - low_battery_img.getWidth()) / 2;
  int16_t y = (DISPLAY_HEIGHT - low_battery_img.getHeight()) / 2;
  display.drawXBitmap(x, y, low_battery_img.getData(), low_battery_img.getWidth(), low_battery_img.getHeight(), SSD1306_WHITE);
  display.display();
  delay(500);
}

void activityLogic() {  
  if (millis() - lastActivity < awakeDuration) {
    checkButton();
    if (!toolSection) {
      listen(mode, debug, lastActivity, awakeDuration);
      mode = -1;
    } else selectDisplayMode();
  } else goToSleep();
}


void setup() {
  //------------- Board initialization ------------------
  initBoard();

  //------------- Display initialization ----------------------
  initDisplay();

  //----------- Initialize variables ---------------
  initSoundAnalysisTools();
  initAlerts();  

  //------------- Verify wake up reason ----------------
  if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0) goToSleep();
  else lastActivity = millis();
}

void loop() {
  if (analogRead(BATTERY_PIN)) activityLogic();
  else printLowBatteryAlert();
}
