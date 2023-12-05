#include "filters.h"

#include <iostream>

const double TotalFilter::freqs1[10] = {0, 48.0, 96.0, 192.0, 384.0, 756.0, 1500.0, 3000.0, 6000.0, 12000.0};
const double TotalFilter::freqs2[10] = {48.0, 96.0, 192.0, 384.0, 756.0, 1500.0, 3000.0, 6000.0, 12000.0, 22050.0};

void BandPass::calculate_h() {
    for (int i=0; i < H_SIZE; i++) {
        int n = i-HALF_WINDOW_SIZE;
        if (n)
            h[i] = gain * (sin(w2*n)-sin(w1*n))/(n*M_PI) * hamming_window(n);
        else
            h[i] = gain*(w2-w1)/M_PI * hamming_window(n);
    }
}

BandPass::BandPass(double freq1, double freq2, double db_gain) :
    h(H_SIZE, 0), freq1(freq1), freq2(freq2), db_gain(db_gain) {
    w1 = hz_to_normalized(freq1);
    w2 = hz_to_normalized(freq2);
    gain = db_to_num(db_gain);

    calculate_h();
}

void BandPass::set_db_gain(double new_gain) {
    db_gain = new_gain;
    gain = db_to_num(db_gain);
    calculate_h();
}

double BandPass::hz_to_normalized(double f) {
    return M_PI*f/FREQ_NYQUIST;
}

double BandPass::db_to_num(double db) {
    return exp10(db/20);
}

double BandPass::hamming_window(int n) {
    return 0.54 + 0.46*cos(n*M_PI/HALF_WINDOW_SIZE);
}

const vector<double> BandPass::get_h() {
    return h;
}

void TotalFilter::initialize() {
    for (int i=0; i < NUM_FILTERS; i++) {
        filters[i] = new BandPass(freqs1[i], freqs2[i], gains_db[i]);
    }
    calculate_h();
}

void TotalFilter::calculate_h() {
    for (int i=0; i < H_SIZE; i++) {
        h[i] = 0;
        for (int j=0; j < NUM_FILTERS; j++) {
            h[i] += filters[j]->get_h()[i];
        }
    }
}

TotalFilter::TotalFilter() : h(H_SIZE, 0) {
    initialize();
}


vector<double> TotalFilter::convolve(const vector<int16_t> &x) {
    int x_size = x.size();
    int result_size = x_size + H_SIZE-1;
    vector<double> out(x_size, 0);


    for (int n=HALF_WINDOW_SIZE; n < x_size+HALF_WINDOW_SIZE; n++) {
        int i = n - HALF_WINDOW_SIZE;
        for (int m=0; m < H_SIZE; m++) {
            if (n-m >= 0 && n-m < x_size) {
                out[i] += x[n-m] * h[m];
            }
            if (out[i] > INT16_MAX) {
                out[i] = INT16_MAX;
            } else if (out[i] < INT16_MIN) {
                out[i] = INT16_MIN;
            }
        }
    }
    return out;
}

void TotalFilter::set_gain(int index, double db_gain) {
    filters[index]->set_db_gain(db_gain);
    calculate_h();
}

TotalFilter::~TotalFilter() {
    for (int i=0 ; i < NUM_FILTERS; i++) {
        delete filters[i];
    }
}
