#include "espectroBarras.h"


const double EspectroBarras::freqs[10] = {32.0, 64.0, 128.0, 256.0, 512.0, 1024.0, 2048.0, 4096.0, 8192.0, 16384.0};


vector<double> EspectroBarras::absDFT(vector<int> x, double normalization) {
    vector<double> out(NUM_FREQS);

    for (int i=0; i < NUM_FREQS; i++) {
        double real = 0;
        double imag = 0;
        for (int n=0; n < x.size(); n++) {
            real += x[n]*cos(M_PI*freqs[i]/FREQ_NYQUIST*n) / static_cast<double>(INT16_MAX);
            imag += x[n]*sin(M_PI*freqs[i]/FREQ_NYQUIST*n) / static_cast<double>(INT16_MAX);
        }
        out[i] = sqrt(real*real + imag*imag) / x.size();
    }
    current_spectro = out;
    return out;
}

string EspectroBarras::show() {
    const int MAX = 8;
    const string BARRA = "▆";

    string out = "";
    for (int i=0; i < MAX; i++) {
        for (double &e : current_spectro) {
            if (e >= 20*exp10(-i)) {
                out += BARRA + "\t";
            } else {
                out += "  \t";
            }
        }
        out += "\n";
    }

    out += "\n";
    out += "0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";

    for (double &e : current_spectro) {
        out += std::to_string(e) + "\t";
    }

    return out;
}
