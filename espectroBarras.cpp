#include "espectroBarras.h"

#include <iostream>

const double EspectroBarras::freqs[10] = {32.0, 64.0, 128.0, 256.0, 512.0, 1024.0, 2048.0, 4096.0, 8192.0, 16384.0};
const double EspectroBarras::freqs1[10] = {0, 48.0, 96.0, 192.0, 384.0, 756.0, 1500.0, 3000.0, 6000.0, 12000.0};
const double EspectroBarras::freqs2[10] = {48.0, 96.0, 192.0, 384.0, 756.0, 1500.0, 3000.0, 6000.0, 12000.0, 22050.0};


vector<double> EspectroBarras::absDFT() {
    fftw_execute(plan);
    vector<double> magnitude(output.size());

    for (int i=0; i < magnitude.size(); i++) {
        magnitude[i] = sqrt(output[i][0]*output[i][0] + output[i][1]*output[i][1]) / output.size();
    }

    for (int i=0; i < NUM_FREQS; i++) {
        double amp_media = 0;
        int initial_k = (magnitude.size() * freqs1[i])/FREQ_AM;
        int final_k = (magnitude.size() * freqs2[i])/FREQ_AM;
        for (int k=initial_k; k < final_k; k ++) {
            amp_media += magnitude[k];
        }
        amp_media /= final_k - initial_k;
        barras[i] = amp_media;
    }
    std::cout << "\n";

    return barras;
}

string EspectroBarras::show() {
    const int MAX = 8;
    const string BARRA = "▆";

    string out = "";
    for (int i=0; i < MAX; i++) {
        for (double &e : barras) {
            if (e >= exp10(-i/1.5)) {
                out += BARRA + "\t";
            } else {
                out += "  \t";
            }
        }
        out += "\n";
    }

    out += "\n";
    out += "0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";

    for (double &e : barras) {
        out += std::to_string(e) + "\t";
    }

    return out;
}

EspectroBarras::EspectroBarras(int N) : input(N), output(N) {
    vector<double> b(10, 0);
    barras = b;

    for (int i=0; i < N; i++) {
        input[i][1] = 0;
    }

    plan = fftw_plan_dft_1d(N, input.data(), output.data(), FFTW_FORWARD, FFTW_ESTIMATE);
}
