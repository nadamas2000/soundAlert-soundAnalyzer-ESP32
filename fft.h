/**
 * @file fft.h
 * @brief FFT
 * 
 * This file contains functions for performing Fast Fourier Transform (FFT)
 * computations and applying windowing functions to input data arrays.
 * The FFT functions implemented here support both forward and inverse
 * transformations, and various windowing functions can be applied to the
 * input data to improve spectral analysis.
 * 
 * The functions included in this file are:
 * 
 * - `double getWeighingFactor(WindowType windowType, unsigned int iteration, unsigned int shiftedLog2N)`
 *   Computes the weighing factor for a specific windowing function at a given iteration.
 * 
 * - `void applyWindow(float _Complex *data, unsigned int log2_N, WindowType windowType, FftDir direction)`
 *   Applies the specified windowing function to the data array.
 * 
 * - `void rearrangeForIFFT(float _Complex *data, unsigned int log2_N)`
 *   Rearranges the data array for the inverse Fast Fourier Transform (FFT) computation.
 * 
 * - `void evaluateFFT(float _Complex *data, unsigned int log2_N, FftDir direction)`
 *   Evaluates the Fast Fourier Transform (FFT) of the input data.
 * 
 * - `void performFFT(float _Complex *data, unsigned int log2_N, FftDir direction)`
 *   Computes the Fast Fourier Transform (FFT) of the input data.
 * 
 * These functions can be used to perform frequency domain analysis, such as
 * spectrum estimation and filtering, on input signals.
 * 
 * @author Nahum Manuel Martín
 * @date 2023/06/25
 */

#pragma once

#include <complex.h>
#include <math.h>

/**
 * @brief Enumeration of the Fourier transform direction.
 */
typedef enum {
  FFT_FORWARD,    ///< Forward transform direction (kernel uses "-1" sign)
  FFT_INVERSE     ///< Inverse transform direction (kernel uses "+1" sign)
} FftDir;

/**
 * @brief Enumeration of the window types.
 */
typedef enum {
    RECTANGLE,            ///< Rectangle window
    HAMMING,              ///< Hamming window
    HANN,                 ///< Hann window
    TRIANGLE,             ///< Triangle window
    NUTTALL,              ///< Nuttall window
    BLACKMAN,             ///< Blackman window
    BLACKMAN_NUTTALL,     ///< Blackman-Nuttall window
    BLACKMAN_HARRIS,      ///< Blackman-Harris window
    FLT_TOP,              ///< Flat top window
    WELCH                 ///< Welch window
} WindowType;

/**
 * @brief Calculates the weighing factor for a specific window type.
 *
 * @param windowType       The type of window.
 * @param iteration        The current iteration.
 * @param shiftedLog2N     The shifted logarithm base 2 of N.
 * @return                 The weighing factor.
 */
double getWeighingFactor(WindowType windowType, unsigned int iteration, unsigned int shiftedLog2N);

/**
 * @brief Applies a window function to the input data.
 *
 * @param data             The input data array.
 * @param log2_N           The logarithm base 2 of the length of the data array.
 * @param windowType       The type of window to be applied.
 * @param direction        The direction of the Fourier transform.
 */
void applyWindow(float _Complex *data, unsigned int log2_N, WindowType windowType, FftDir direction);

/**
 * @brief Rearranges the input data for the Inverse Fast Fourier Transform (IFFT).
 *
 * @param data             The input data array.
 * @param log2_N           The logarithm base 2 of the length of the data array.
 */
void rearrangeForIFFT(float _Complex *data, unsigned int log2_N);

/**
 * @brief Computes the Fast Fourier Transform (FFT) of the input data.
 *
 * @param data             The input data array.
 * @param log2_N           The logarithm base 2 of the length of the data array.
 * @param direction        The direction of the Fourier transform.
 */
void evaluateFFT(float _Complex *data, unsigned int log2_N, FftDir direction);

/**
 * @brief Computes the Fast Fourier Transform (FFT) of the input data with additional operations.
 *
 * @param data             The input data array.
 * @param log2_N           The logarithm base 2 of the length of the data array.
 * @param direction        The direction of the Fourier transform.
 */
void performFFT(float _Complex *data, unsigned int log2_N, FftDir direction);


