#include <iostream>
#include <cmath>
#include "filters.h"

int main() {
    TotalFilter teste;

    vector<double> input(1000, 1);
    for (int i=0; i < 1000; i++) {
        input[i] = 2*cos(i*M_PI) + cos(i*M_PI/2);
        // input[i] = cos(i*M_PI);
        std::cout << input[i] << ", ";
    }
    std::cout << "\n";

    vector<double> output = teste.convolve(input);
    // for (double x : output) {
    //     std::cout << x <<  ", ";
    // }
    // std::cout << "\n";

    for (int i=0; i < 1000; i++) {
        std::cout << 2*cos(i*M_PI) << ", ";
    }
    std::cout << "\n";

    teste.set_gain(1, -40);
    teste.set_gain(2, -40);
    teste.set_gain(3, -40);
    teste.set_gain(4, -40);
    teste.set_gain(5, -40);
    teste.set_gain(6, -40);
    teste.set_gain(7, -40);
    teste.set_gain(8, -40);
    output = teste.convolve(input);
    for (double x : output) {
        std::cout << x <<  ", ";
    }
    std::cout << "\n";


    return 0;
}
