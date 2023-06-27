#include "fft.h"

double getWeighingFactor(WindowType windowType, unsigned int iteration, unsigned int shiftedLog2N) {
  double ratio = (static_cast<double>(iteration) / (shiftedLog2N - 1));
  // Compute and record weighting factor
  switch (windowType) {    
    case HAMMING: // hamming
      return 0.54 - (0.46 * cos(2 * M_PI * ratio));
    case HANN: // hann
      return 0.54 * (1.0 - cos(2 * M_PI * ratio));
    case TRIANGLE: // triangle (Bartlett)
      return 1.0 - ((2.0 * abs(static_cast<double>(iteration) - ((shiftedLog2N - 1) / 2.0))) / (shiftedLog2N - 1));
    case NUTTALL: // nuttall
      return 0.355768 - (0.487396 * (cos(2 * M_PI * ratio))) + (0.144232 * (cos(4 * M_PI * ratio))) - (0.012604 * (cos(6 * M_PI * ratio)));
    case BLACKMAN: // blackman
      return 0.42323 - (0.49755 * (cos(2 * M_PI * ratio))) + (0.07922 * (cos(4 * M_PI * ratio)));
    case BLACKMAN_NUTTALL: // blackman nuttall
      return 0.3635819 - (0.4891775 * (cos(2 * M_PI * ratio))) + (0.1365995 * (cos(4 * M_PI * ratio))) - (0.0106411 * (cos(6 * M_PI * ratio)));
    case BLACKMAN_HARRIS: // blackman harris
      return 0.35875 - (0.48829 * (cos(2 * M_PI * ratio))) + (0.14128 * (cos(4 * M_PI * ratio))) - (0.01168 * (cos(6 * M_PI * ratio)));
    case FLT_TOP: // flat top
      return 0.2810639 - (0.5208972 * cos(2 * M_PI * ratio)) + (0.1980399 * cos(4 * M_PI * ratio));
    case WELCH: // welch
      return 1.0 - ((static_cast<double>(iteration) - (shiftedLog2N - 1) / 2.0) / ((shiftedLog2N - 1) / 2.0) *
        (static_cast<double>(iteration) - (shiftedLog2N - 1) / 2.0) / ((shiftedLog2N - 1) / 2.0));
    default: // default RECTANGLE
      return 1.0;
  }
}

void applyWindow(float _Complex *data, unsigned int log2_N, WindowType windowType, FftDir direction) {
  unsigned int N = 1 << log2_N;
  unsigned int Nd2 = N >> 1;
  for (int i = 0; i < Nd2; i++) {
    if (direction == FFT_FORWARD) {
      data[i] *= getWeighingFactor(windowType, i, N);
      data[N - (i + 1)] *= getWeighingFactor(windowType, i, N);
    }
    else {
      data[i] /= getWeighingFactor(windowType, i, N);
      data[N - (i + 1)] /= getWeighingFactor(windowType, i, N);
    }
  }
}

void rearrangeForIFFT(float _Complex *data, unsigned int log2_N) {
  unsigned int N = 1 << log2_N;   // N
  unsigned int Nd2 = N >> 1;      // N/2 = number range midpoint
  unsigned int Nm1 = N - 1;       // N-1 = digit mask

  for (unsigned int i, j = 0; i < N; i++) {
    if (j > i) {
      float _Complex tmp = data[i];
      data[i] = data[j];
      data[j] = tmp;
    }
    unsigned int lszb = ~i & (i + 1);
    unsigned int mszb = Nd2 / lszb;
    unsigned int bits = Nm1 & ~(mszb - 1);
    j ^= bits;
  }
}

void evaluateFFT(float _Complex *data, unsigned int log2_N, FftDir direction) {
  unsigned int N = 1 << log2_N;
  double theta_2pi = (direction == FFT_FORWARD) ? -M_PI : M_PI;
  theta_2pi *= 2;

  for (unsigned int r = 1; r <= log2_N; r++) {
    unsigned int m = 1 << r;
    unsigned int md2 = m >> 1;
    double theta = theta_2pi / m;
    double re = cos(theta);
    double im = sin(theta);
    double _Complex Wm = re + I * im;
    for (unsigned int n = 0; n < N; n += m) {
      double _Complex Wmk = 1.0f;   // Use double for precision
      for (unsigned int k = 0; k < md2; k++) {
        unsigned int i_e = n + k;
        unsigned int i_o = i_e + md2;
        double _Complex t = Wmk * data[i_o];
        data[i_o] = data[i_e] - t;
        data[i_e] = data[i_e] + t;
        t = Wmk * Wm;
        Wmk = t;
      }
    }
  }
}

void performFFT(float _Complex *data, unsigned int log2_N, FftDir direction) {
  rearrangeForIFFT(data, log2_N);
  evaluateFFT(data, log2_N, direction);
  data[0] *= 0.0; // 0 Hz not exists, data error.
}