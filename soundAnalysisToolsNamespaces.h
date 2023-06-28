/**
 * @file soundAnalysisToolsNamespaces.h
 * @brief Sound Analysis Tools Namespaces
 *
 * This file defines several namespaces for common sound analysis tools, display tools, and spectrum tools.
 * The namespaces include variables and functions related to each specific area of functionality.
 *
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#pragma once

#include "board.h"
#include "fft.h"

/**
 * @namespace commonSoundAnalysisTools
 * @brief Namespace for common sound analysis tools.
 */
namespace commonSoundAnalysisTools {
  long chrono; /**< Chrono variable. */
  String title[7] = {""}; /**< Title array. */
}

/**
 * @namespace commonDisplays
 * @brief Namespace for common display tools.
 */
namespace commonDisplays {
  unsigned short hOffset; /**< Horizontal offset variable. */
  unsigned short graphH; /**< Graph height variable. */
}

/**
 * @namespace minMax
 * @brief Namespace for minimum and maximum values.
 */
namespace minMax {
  short ampMax = 0; /**< Maximum amplitude variable. */
  short ampMin = 4095; /**< Minimum amplitude variable. */
}

/**
 * @namespace commonSpectrum
 * @brief Namespace for common spectrum tools.
 */
namespace commonSpectrum {
  long chronoRead; /**< Chrono read variable. */
  const unsigned char MAX_FREQ = 16; /**< Maximum frequency (kHz). */
  unsigned long sampling_period_us = round(1000ul * (1.0 / MAX_FREQ)); /**< Sampling period in microseconds. */

  /**
   * @brief Acquires sound data.
   * @param data Pointer to complex sound data.
   * @param nSamples Number of samples to acquire.
   */
  void acquireSound(float _Complex *data, int nSamples) {
    for (int i = 0; i < nSamples; i++) {
      chronoRead = micros();
      data[i] = analogRead(MIC_PIN);
      while (micros() - chronoRead < sampling_period_us); // only if analogRead time < sampling_period_us
    } // 12.8 ms
  }

  void getData(float _Complex *data, int nSamples, int log2Sample) {
    acquireSound(data, nSamples);
    applyWindow (data, log2Sample, HAMMING, FFT_FORWARD);
    performFFT(data, log2Sample, FFT_FORWARD);    
  }
}