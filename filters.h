#ifndef FILTERS_H_
#define FILTERS_H_

#include <vector>
#include <cmath>
using std::vector;

const int HALF_WINDOW_SIZE = 200;
const int H_SIZE = 2*HALF_WINDOW_SIZE + 1;
const double FREQ_AM = 44100;  // Hz
const double FREQ_NYQUIST = FREQ_AM/2;  // Hz

class BandPass {
 private:
     vector<double> h;
     double db_gain, gain;
     double freq1, freq2, w1, w2;
     static double hz_to_normalized(double f);
     static double db_to_num(double db);
     static double hamming_window(int n);
     void calculate_h();
 public:
     BandPass(double freq1, double freq2, double db_gain);
     const vector<double> get_h();
     void set_db_gain(double new_gain);
};

class TotalFilter {
 private:
     static const int NUM_FILTERS = 10;
     BandPass* filters[NUM_FILTERS];
     static const double freqs1[NUM_FILTERS];
     static const double freqs2[NUM_FILTERS];
     double gains_db[NUM_FILTERS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
     void calculate_h();
     vector<double> h;
     void initialize();

 public:
     TotalFilter();
     vector<double> convolve(const vector<int16_t> &x);
     void set_gain(int index, double db_gain);
};

#endif  //  FILTERS_H_
