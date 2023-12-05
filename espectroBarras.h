#ifndef ESPECTROBARRAS_H_
#define ESPECTROBARRAS_H_

#include <vector>
#include <cmath>
#include <string>
#include <fftw3.h>

using std::vector;
using std::string;

class EspectroBarras {
 private:
    static const int NUM_FREQS = 10;
    static const double freqs[NUM_FREQS];
    static constexpr double FREQ_AM = 44100;  // Hz
    static constexpr double FREQ_NYQUIST = FREQ_AM/2;  // Hz

    static const double freqs1[NUM_FREQS];
     static const double freqs2[NUM_FREQS];

    fftw_plan plan;

    vector<fftw_complex> output;
 public:
    vector<double> barras;
    vector<fftw_complex> input;
    vector<double> absDFT();
    string show();
    EspectroBarras(int N);
};

#endif  //  ESPECTROBARRAS_H_
