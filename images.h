/**
 * @file images.h
 * @brief XBM Images Library
 *
 * This library provides XBM images and their corresponding data for various icons.
 * 
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#pragma once

/**
 * @class Xbm
 * @brief Represents an X Bitmap image.
 */
class Xbm {
private:
  short width; /**< The width of the image. */
  short height; /**< The height of the image. */
  const unsigned char* data; /**< The pixel data of the image. */

public:
  /**
   * @brief Constructs an Xbm object.
   * @param w The width of the image.
   * @param h The height of the image.
   * @param d The pixel data of the image.
   */
  Xbm(short w, short h, const unsigned char* d)
    : width(w), height(h), data(d) {}

  /**
   * @brief Gets the width of the image.
   * @return The width of the image.
   */
  short getWidth() const {
    return width;
  }

  /**
   * @brief Gets the height of the image.
   * @return The height of the image.
   */
  short getHeight() const {
    return height;
  }

  /**
   * @brief Gets the pixel data of the image.
   * @return The pixel data of the image.
   */
  const unsigned char* getData() const {
    return data;
  }
};

// Empty image
const unsigned char empty_data [] PROGMEM = {};
const Xbm empty_img PROGMEM (0, 0, empty_data);

// Phone image
const unsigned char phone_data [] PROGMEM = {
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0xC0, 0x1F, 
  0x00, 0x00, 0xE0, 0x3F, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0xF0, 0x3F, 
  0x00, 0x00, 0xF8, 0x3F, 0x00, 0x00, 0xF8, 0x3F, 0x00, 0x00, 0xF8, 0x3F, 
  0x00, 0x00, 0xF0, 0x1F, 0x00, 0x00, 0xE0, 0x1F, 0x00, 0x00, 0xC0, 0x0F, 
  0x00, 0x00, 0xC0, 0x0F, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0xE0, 0x07, 
  0x00, 0x00, 0xF0, 0x07, 0x00, 0x00, 0xF8, 0x03, 0x00, 0x00, 0xFC, 0x01, 
  0xC0, 0x03, 0xFE, 0x00, 0xE0, 0x07, 0x7F, 0x00, 0xF0, 0x8F, 0x3F, 0x00, 
  0xF8, 0xEF, 0x1F, 0x00, 0xFC, 0xFF, 0x0F, 0x1F, 0xFE, 0xFF, 0xC7, 0x31, 
  0xFE, 0xFF, 0x63, 0x20, 0xFE, 0xFF, 0x31, 0x60, 0xFC, 0x7F, 0x10, 0x40, 
  0xFC, 0x1F, 0x18, 0x40, 0xF6, 0x0F, 0x0C, 0x40, 0xE2, 0x00, 0x06, 0x40, 
  0x03, 0x00, 0x03, 0x00, 0x02, 0x80, 0x01, 0x00, 0x06, 0xC0, 0x00, 0x00, 
  0x1C, 0x70, 0x00, 0x00, 0xF0, 0x1F, 0x00, 0x00, };
const Xbm phone_img PROGMEM (31, 35, phone_data);

// Door image
const unsigned char door_data [] PROGMEM = {
  0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0x3F, 0x03, 0x00, 0x30, 0x03, 0x00, 0x30, 
  0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 
  0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 
  0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 
  0x93, 0xFF, 0x33, 0x93, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 
  0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 
  0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 
  0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, 0xF3, 0xFF, 0x33, };
const Xbm door_img PROGMEM (22, 31, door_data);

// Bell image
const unsigned char bell_data [] PROGMEM = {
  0x00, 0x1C, 0x0E, 0x00, 0x80, 0x07, 0x78, 0x00, 0xE0, 0x00, 0xC0, 0x01, 
  0x30, 0x00, 0x00, 0x03, 0x18, 0x1C, 0x0E, 0x06, 0x0C, 0x07, 0x38, 0x0C, 
  0x84, 0x01, 0x60, 0x08, 0xC6, 0xE0, 0xC1, 0x18, 0x62, 0xF0, 0x83, 0x11, 
  0x23, 0xF8, 0x07, 0x31, 0x31, 0xFC, 0x0F, 0x23, 0x11, 0xFE, 0x1F, 0x22, 
  0x11, 0xFF, 0x3F, 0x22, 0x11, 0xFF, 0x3F, 0x22, 0x80, 0xFF, 0x7F, 0x00, 
  0x80, 0xFF, 0x7F, 0x00, 0x80, 0xFF, 0x7F, 0x00, 0x80, 0xFF, 0x7F, 0x00, 
  0x80, 0xFF, 0x7F, 0x00, 0x80, 0xFF, 0x7F, 0x00, 0x80, 0xFF, 0x7F, 0x00, 
  0x80, 0xFF, 0x7F, 0x00, 0x80, 0xFF, 0x7F, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 
  0xC0, 0xFF, 0xFF, 0x00, 0xC0, 0xFF, 0xFF, 0x00, 0xE0, 0xFF, 0xFF, 0x01, 
  0xF0, 0xFF, 0xFF, 0x03, 0x00, 0x30, 0x03, 0x00, 0x00, 0xF0, 0x03, 0x00, 
  0x00, 0xC0, 0x00, 0x00, };
const Xbm bell_img PROGMEM (30, 31, bell_data);

// Low battery image
const unsigned char low_battery_data [] PROGMEM = {
  0xFC, 0xFF, 0xFF, 0xFF, 0x00, 0x06, 0x00, 0x00, 0xC0, 0x01, 0x03, 0x00, 
  0x00, 0x80, 0x01, 0xDB, 0x00, 0x00, 0x00, 0x01, 0xDB, 0x00, 0x00, 0x00, 
  0x01, 0xDB, 0x00, 0x00, 0x00, 0x01, 0xDB, 0x00, 0x00, 0x00, 0x0F, 0xDB, 
  0x00, 0x00, 0x00, 0x0F, 0xDB, 0x00, 0x00, 0x00, 0x0C, 0xDB, 0x00, 0x00, 
  0x00, 0x0C, 0xDB, 0x00, 0x00, 0x00, 0x0C, 0xDB, 0x00, 0x00, 0x00, 0x0C, 
  0xDB, 0x00, 0x00, 0x00, 0x0F, 0xDB, 0x00, 0x00, 0x00, 0x0F, 0xDB, 0x00, 
  0x00, 0x00, 0x01, 0xDB, 0x00, 0x00, 0x00, 0x01, 0xDB, 0x00, 0x00, 0x00, 
  0x01, 0x03, 0x00, 0x00, 0x80, 0x01, 0x06, 0x00, 0x00, 0xC0, 0x01, 0xFC, 
  0xFF, 0xFF, 0xFF, 0x00, };
const Xbm low_battery_img PROGMEM (36, 20, low_battery_data);

// Microphone logo image
const unsigned char mic_logo_data [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0x00, 0x00, 0x02, 0x04, 0x00, 0x10, 0x00, 0x00, 0x08, 0x80, 
  0x00, 0x00, 0x02, 0x04, 0x00, 0x10, 0x00, 0x01, 0x08, 0x80, 0x00, 0x00, 
  0x02, 0x04, 0x00, 0x10, 0x00, 0x01, 0x1C, 0xC0, 0x21, 0x00, 0x02, 0x04, 
  0x40, 0x38, 0x80, 0x03, 0x14, 0x41, 0x21, 0x00, 0x02, 0x04, 0x40, 0x28, 
  0x88, 0x02, 0x34, 0x61, 0x21, 0x00, 0x02, 0x04, 0x40, 0x68, 0xC8, 0x02, 
  0xA4, 0x23, 0x73, 0x3C, 0x02, 0xC4, 0xE3, 0x4C, 0x5C, 0x02, 0xA6, 0x22, 
  0xD2, 0x24, 0x02, 0x44, 0xB2, 0x44, 0x54, 0x06, 0xE3, 0x26, 0x9A, 0x25, 
  0x02, 0x44, 0x9A, 0x45, 0x76, 0x0C, 0xC0, 0x34, 0x0A, 0x24, 0x02, 0x44, 
  0x02, 0xC5, 0x32, 0x00, 0x80, 0x14, 0x0E, 0x24, 0x02, 0x44, 0x02, 0x87, 
  0x12, 0x00, 0x80, 0x14, 0x04, 0x24, 0x02, 0x44, 0x02, 0x82, 0x12, 0x00, 
  0x00, 0x1C, 0x04, 0x24, 0x02, 0x44, 0x02, 0x82, 0x03, 0x00, 0x00, 0x18, 
  0x04, 0x24, 0x02, 0x44, 0x02, 0x82, 0x01, 0x00, 0x00, 0x08, 0x00, 0x24, 
  0x02, 0x44, 0x02, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00, 0x6C, 0x06, 0x66, 
  0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x48, 0x0C, 0x23, 0x01, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xC8, 0xF8, 0x31, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x98, 0x01, 0x98, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x30, 0x03, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0xFE, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x30, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x1C, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0x0F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xC0, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 
  0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x1F, 
  0x00, 0x00, 0x00, 0x00, };
const Xbm mic_logo_img PROGMEM (76, 40, mic_logo_data);

// Arrow to right image
const unsigned char arrow_right_data [] PROGMEM = {
  0x40, 0x00, 0x00, 0xC0, 0x00, 0x00, 0xC0, 0x03, 0x00, 0xC0, 0x07, 0x00, 
  0xC0, 0x1F, 0x00, 0xC0, 0x3F, 0x00, 0xC0, 0xFF, 0x00, 0xFF, 0xFF, 0x01, 
  0xFF, 0xFF, 0x07, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0x7F, 
  0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0x07, 
  0xFF, 0xFF, 0x01, 0xC0, 0x7F, 0x00, 0xC0, 0x3F, 0x00, 0xC0, 0x0F, 0x00, 
  0xC0, 0x07, 0x00, 0xC0, 0x01, 0x00, 0xC0, 0x00, 0x00, };
const Xbm arrow_right_img PROGMEM (23, 23, arrow_right_data);

// Arrow to left image
const unsigned char arrow_left_data [] PROGMEM = {
  0x00, 0x00, 0x01, 0x00, 0x80, 0x01, 0x00, 0xE0, 0x01, 0x00, 0xF0, 0x01, 
  0x00, 0xFC, 0x01, 0x00, 0xFE, 0x01, 0x80, 0xFF, 0x01, 0xC0, 0xFF, 0x7F, 
  0xF0, 0xFF, 0x7F, 0xF8, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 0xFF, 0xFF, 0x7F, 
  0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 0xF8, 0xFF, 0x7F, 0xF0, 0xFF, 0x7F, 
  0xC0, 0xFF, 0x7F, 0x00, 0xFF, 0x01, 0x00, 0xFE, 0x01, 0x00, 0xF8, 0x01, 
  0x00, 0xF0, 0x01, 0x00, 0xC0, 0x01, 0x00, 0x80, 0x01, };
const Xbm arrow_left_img PROGMEM (23, 23, arrow_left_data);

// Arrow to up image
const unsigned char arrow_up_data [] PROGMEM = {
  0x00, 0x0C, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x3F, 0x00, 
  0x80, 0x7F, 0x00, 0x80, 0x7F, 0x00, 0xC0, 0xFF, 0x00, 0xC0, 0xFF, 0x01, 
  0xE0, 0xFF, 0x01, 0xF0, 0xFF, 0x03, 0xF0, 0xFF, 0x07, 0xF8, 0xFF, 0x07, 
  0xFC, 0xFF, 0x0F, 0xFC, 0xFF, 0x1F, 0xFE, 0xFF, 0x1F, 0xFF, 0xFF, 0x3F, 
  0xFF, 0xFF, 0x7F, 0xC0, 0xFF, 0x00, 0xC0, 0xFF, 0x00, 0xC0, 0xFF, 0x00, 
  0xC0, 0xFF, 0x00, 0xC0, 0xFF, 0x00, 0xC0, 0xFF, 0x00, };
const Xbm arrow_up_img PROGMEM (23, 23, arrow_up_data);

// Arrow to down image
const unsigned char arrow_down_data [] PROGMEM = {
  0x80, 0xFF, 0x01, 0x80, 0xFF, 0x01, 0x80, 0xFF, 0x01, 0x80, 0xFF, 0x01, 
  0x80, 0xFF, 0x01, 0x80, 0xFF, 0x01, 0xFF, 0xFF, 0x7F, 0xFE, 0xFF, 0x7F, 
  0xFC, 0xFF, 0x3F, 0xFC, 0xFF, 0x1F, 0xF8, 0xFF, 0x1F, 0xF0, 0xFF, 0x0F, 
  0xF0, 0xFF, 0x07, 0xE0, 0xFF, 0x07, 0xC0, 0xFF, 0x03, 0xC0, 0xFF, 0x01, 
  0x80, 0xFF, 0x01, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x7E, 0x00, 
  0x00, 0x3C, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x18, 0x00, };
const Xbm arrow_down_img PROGMEM (23, 23, arrow_down_data);