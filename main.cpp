#include <iostream>
#include <cmath>
#include "./filters.h"
#include "./inputController.h"
#include "./outputController.h"

int main() {

    // string file_path = "Queen-–-Bohemian-Rhapsody-_Official-Video-Remastered_.wav";
    // InputController input_controller;

    // if (!input_controller.setFile(file_path)) {
    //     std::cout << "Deu Erro\n";
    //     return -1;
    // }

    // TotalFilter filter;
    // vector<double> output_buffer1(BUFFER_SIZE);
    // vector<double> output_buffer2(BUFFER_SIZE);

    // while (input_controller.read_file()) {
    //     // std::cout << "Input: " << input_controller.getChannel(0)[0] <<  " " << input_controller.getChannel(1)[0] << std::endl;
    //     output_buffer1 = filter.convolve(input_controller.getChannel(0));
    //     output_buffer2 = filter.convolve(input_controller.getChannel(1));
    //     // std::cout << "Output: " << output_buffer1[HALF_WINDOW_SIZE] <<  " " << output_buffer2[HALF_WINDOW_SIZE] << std::endl;
    // }
    // input_controller.close_file();

    OutputController output_controller(256, 2, 44100);
    if (!output_controller.open_stream()) return -1;
    output_controller.play();
    if (!output_controller.close_stream()) return -1;
    return 0;
}
