/**
* @file listenLogic.h
* @brief Logic of listening mode
*
* This library provides the logic about listening mode.
* 
* @author Nahum Manuel Martín
* @date 2023/06/25
*/

#include "board.h"
#include "display.h"
#include "alerts.h"
#include "fft.h" // Fast Fourier Transform Algorithms

// -------------- Listening global variables and constants ------------------
const int LISTEN_SAMPLES = 1024;
const int LISTEN_MAX_FREQ = 16; // kHz
const unsigned long sampling_period_us_LISTEN = round(1000ul * (1.0 / LISTEN_MAX_FREQ)); // 1/Hz = T(s) -> 1/kHz = T(ms)
const int log2Sample_LISTEN = log(LISTEN_SAMPLES) / log(2);

bool alert = false;
long chrono_LISTEN;
float _Complex data_LISTEN[LISTEN_SAMPLES];
float max_LISTEN = 0;
int maxI_LISTEN = 0;



// ----------------- Main listening mode -----------------
/**
* @brief Performs the main listening mode operation.
* @param mode The current display mode.
* @param debug Debug mode to show technical information
*/
void listen(int mode, bool debug, unsigned long &lastActivity, int &awakeDuration);



// ---------------- Printing images and info --------------------
/**
* @brief Displays the listening logo on the display.
*/
void printListeningLogo();

/**
* @brief Draws the alert images on the display for a given AlertElement.
* @param a The AlertElement containing the image properties.
*/
void drawAlertImages(AlertElement &a);

/**
* @brief Displays the alert information on the display.
*/
void printAlert(bool debug);

/**
* @brief Displays the relevant information for the listening mode on the display.
*/
void showListeningInfo();


// ---------------- Sound analyze ----------------------
/**
* @brief Reads the sound data from the microphone.
*/
void getSound();

/**
* @brief Analyzes the sound data using Fast Fourier Transform (FFT).
*/
void analyzeSound();

/**
* @brief Extracts the relevant information from the analyzed sound data.
*/
void getRellevantInfo();


// --------------- Check alerts ------------------------
/**
* @brief Checks if there is a match between the analyzed sound data and the defined alerts.
*/
void alertMatching(unsigned long &lastActivity, int &awakeDuration);



// ---------------- Printing images and info --------------------
void printListeningLogo() {
  int xCenter = (DISPLAY_WIDTH - mic_logo_img.getWidth()) / 2;
  int yCenter = (DISPLAY_HEIGHT - mic_logo_img.getHeight()) / 2;
  display.clearDisplay();
  display.drawXBitmap(xCenter, yCenter, mic_logo_img.getData(), mic_logo_img.getWidth(), mic_logo_img.getHeight(), SSD1306_WHITE);
  display.display();
}

void drawAlertImages(AlertElement &a) {
  display.drawXBitmap(
    a.image1_xPos,
    a.image1_yPos,
    a.image1->getData(),
    a.image1->getWidth(),
    a.image1->getHeight(),
    SSD1306_WHITE
  );
  display.drawXBitmap(
    a.image2_xPos,
    a.image2_yPos,
    a.image2->getData(),
    a.image2->getWidth(),
    a.image2->getHeight(),
    SSD1306_WHITE
  );
}

void printAlert(bool debug) {
  display.clearDisplay();
  if (debug) {
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);  
    for (int i = 0; i < N_ALERT_TYPES; i++) {
      if (alerts[i].alertStatus) {
        display.setCursor(0, 0);
        display.println("Alert! " + String(i));
        display.setCursor(0, FONT_HEIGHT);
        display.println("Hz: " + String(alerts[i].freq));
        display.setCursor(0, FONT_HEIGHT * 2);
        display.println("Mark: " + String(alerts[i].iteratorMark));
        display.setCursor(0, FONT_HEIGHT * 3);
        display.println("Intensity: " + String(alerts[i].intensityMark));
      }
    }  
  } else {
    for (int i = 0; i < N_ALERT_TYPES; i++) {
      if (alerts[i].alertStatus) drawAlertImages(alerts[i]);
    }
  }
  
  display.display();
}

void showListeningInfo() {
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(0, 0);
  display.println("Hz: " + String(maxI_LISTEN));  
  display.setCursor(0, FONT_HEIGHT);
  display.println("AHz: " + String(max_LISTEN));
  display.setCursor(0, FONT_HEIGHT * 2);
  display.println("Hz: " + String(maxI_LISTEN * 15.2256 * (1024.0 / LISTEN_SAMPLES)));
  display.display();
}


// ---------------- Sound analyze ----------------------
void getSound() {
  for (int i = 0; i < LISTEN_SAMPLES; i++) {
    chrono_LISTEN = micros();
    data_LISTEN[i] = analogRead(MIC_PIN);    
    while (micros() - chrono_LISTEN < sampling_period_us_LISTEN); // only if analogRead time < sampling_period_us
  }
}

void analyzeSound() {
  getSound();   
  applyWindow (data_LISTEN, log2Sample_LISTEN, HAMMING, FFT_FORWARD);
  performFFT(data_LISTEN, log2Sample_LISTEN, FFT_FORWARD);
}

void getRellevantInfo() {
  max_LISTEN = 0;
  maxI_LISTEN = 0;
  for (int i = 1; i < LISTEN_SAMPLES; i++) {
    if (creal(data_LISTEN[i]) > max_LISTEN) {
      max_LISTEN = creal(data_LISTEN[i]);
      maxI_LISTEN = i;    
    }
  }
}


// --------------- Check alerts ------------------------
void alertMatching(unsigned long &lastActivity, int &awakeDuration) {
  bool alertMatch = false;
  for (int i = 0; i < N_ALERT_TYPES; i++) {
    if (maxI_LISTEN == alerts[i].iteratorMark and max_LISTEN > alerts[i].minIntensity){
      alertMatch = true;
      alerts[i].alertStatus = true; 
      alerts[i].intensityMark = max_LISTEN;
      for (int j = 0; j < N_ALERT_TYPES; j++) {
        if (j != i) alerts[j].alertStatus = false;
      }
    }
  }

  if (alertMatch) {
    alert = true;
    lastActivity = millis();
    awakeDuration = (2 * 60 * 1000); // 2 minutes showing alert
  }
}


// ----------------- Main listening mode -----------------
void listen(int mode, bool debug, unsigned long &lastActivity, int &awakeDuration) {  
  if (!alert and mode != -1) printListeningLogo();
  
  analyzeSound();
  getRellevantInfo();
  alertMatching(lastActivity, awakeDuration);
  if (alert) printAlert(debug);

  // info in listening mode.
  if (debug and !alert) showListeningInfo();
  
}