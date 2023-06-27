# soundAlert-soundAnalyzer ESP32
 Alert system by frequency sound with sound analyzing tools. Made for Wifi Kit 32 V3 (ESP32)

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

--------------------------------------

<img src="https://cdn.thingiverse.com/assets/96/c9/16/7f/d5/44437ce8-861f-498e-a02c-16e2b9ba65a3.jpg" alt="drawing" width="50%" height="50%"/>

[Video with sound analysis tool working demo (old release)](https://www.youtube.com/watch?v=icu3Co2jCHw)

-------------------------------------

[3D Printed Case in Thingiverse](https://www.thingiverse.com/thing:6096022)

![3D Printed Case](https://cdn.thingiverse.com/assets/06/06/ce/4a/9f/featured_preview_50df27e4-ebd3-459a-aac1-78d2d78a0f4c.jpg)

[Video with sound analysis tool working in case](https://www.youtube.com/watch?v=QHgimJAhCMs)

-------------------------------------

AAA Rechargable Battery connections for 3.7V or 1.2V.

![AAA Battery connections](https://cdn.thingiverse.com/assets/18/5c/7b/80/aa/c5b2b958-8ccc-4ef4-b754-989f01366405.png)
