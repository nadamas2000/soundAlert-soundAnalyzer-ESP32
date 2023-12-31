### soundAlert-soundAnalyzer ESP32
 Alert system by frequency sound with sound analyzing tools. Made for Wifi Kit 32 V3 (ESP32)

# Sound Alert Detector / Sound Analyzer - Wifi Kit 32 V3

Description:
The Sound Analyzer Wifi Kit 32 is a program that utilizes Arduino and various libraries to create a sound alert system. The main functionality of the code is to detect specific frequencies with certain intensities and display corresponding alerts on an OLED display.

## Hardware

The system employs the following hardware components:

- Microcontroller Board: Heltec Wifi Kit 32 V3
- Microphone: Max9814
- Battery Holder: AA or 18650 Battery Holder
- Case: 3D printed case

## Dependencies

The program relies on the following libraries:

- Arduino libraries
- SPI
- Wire
- Driver RTC IO (ESP32)
- Adafruit SSD1306 OLED display library
- Math
- Complex

## Functionality

The program consists of the following sections:

- Hardware configuration: This section includes the setup of the microcontroller board and microphone.
- Display initialization: The OLED display is initialized in this section.
- Sound analysis tools: The program performs fast Fourier transform (FFT) analysis on the captured sound data to identify specific frequencies and intensity thresholds.
- Main loop functionality: The system continuously captures sound, performs analysis, and triggers alerts when detections occur. The corresponding messages are displayed on the OLED screen.

## Connetions Schema
- 22 AWG flexible cable
- Do not use on-board Dupont pins

<img src="https://cdn.thingiverse.com/assets/76/e9/e6/0c/3c/e8dd050e-24fb-42cf-af6b-cd4d3ed9ac02.png" alt="drawing" width="50%" height="50%"/>\

## Battery Connection
Rechargable Battery connections for 3.7V or 1.2V.

![AA Battery connections](https://cdn.thingiverse.com/assets/18/5c/7b/80/aa/c5b2b958-8ccc-4ef4-b754-989f01366405.png)

## Working Connected

[Video with sound detection and frequencies associated to bells or alarms](https://www.youtube.com/watch?v=QawYXSqPsZ8)\
[Video with sound analysis tool working demo (old release)](https://www.youtube.com/watch?v=icu3Co2jCHw)

## 3D Printed Case
[3D Printed Case on Thingiverse.](https://www.thingiverse.com/thing:6096022)\
3xAA Battery model and 1x18650 Battery model.

<img src="https://cdn.thingiverse.com/assets/ab/04/84/ee/ff/large_display_50df27e4-ebd3-459a-aac1-78d2d78a0f4c.jpg" alt="drawing" width="50%" height="50%"/>
<img src="https://cdn.thingiverse.com/assets/96/c9/16/7f/d5/44437ce8-861f-498e-a02c-16e2b9ba65a3.jpg" alt="drawing" width="50%" height="50%"/>
<img src="https://cdn.thingiverse.com/assets/cf/9a/db/d3/e5/large_display_0e1526ad-7a28-4839-be75-506c2aca9819.png" alt="drawing" width="50%" height="50%"/>

[Video with sound analysis tool working in case](https://www.youtube.com/watch?v=QHgimJAhCMs)

## Author

Nahúm Manuel Martín

## References

- Heltec Wifi Kit 32 V3 board: https://heltec.org/project/wifi-kit-32-v3/
- Microphone Max9814: https://www.analog.com/media/en/technical-documentation/data-sheets/max9814.pdf
- Adafruit SSD1306 Library: https://github.com/adafruit/Adafruit_SSD1306/
- Arduino Libraries: https://www.arduino.cc/reference/en/libraries/
