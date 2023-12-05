#ifndef ESPECTROBARRAS_H_
#define ESPECTROBARRAS_H_

#include <vector>
#include <cmath>
#include <string>

using std::vector;
using std::string;

class EspectroBarras {
 private:
    static const int NUM_FREQS = 10;
    static const double freqs[NUM_FREQS];
    static constexpr double FREQ_AM = 44100;  // Hz
    static constexpr double FREQ_NYQUIST = FREQ_AM/2;  // Hz
 public:
    vector<double> current_spectro;
    vector<double> absDFT(vector<int> x , double normalization);
    string show();
};

#endif  //  ESPECTROBARRAS_H_
