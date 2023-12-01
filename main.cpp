#include <iostream>
#include "./filters.h"
#include "./inputController.h"
#include "./outputController.h"


int main() {
    const string file_input = "Queen – Bohemian Rhapsody (Official Video Remastered).wav";
    InputController input_controller;
    input_controller.setFile(file_input);
    SF_INFO file_info = input_controller.getFileInfo();

    TotalFilter filter;

    filter.set_gain(0, 0);
    filter.set_gain(1, 0);
    filter.set_gain(2, 0);
    filter.set_gain(3, 0);
    filter.set_gain(4, 0);
    filter.set_gain(5, 0);
    filter.set_gain(6, 6);
    filter.set_gain(7, 0);
    filter.set_gain(8, 0);
    filter.set_gain(9, 0);


    vector<sf::Int16> test_audio(BUFFER_SIZE*2);

    vector<int16_t> *input_channel0 = input_controller.getChannel(0);
    vector<int16_t> *input_channel1 = input_controller.getChannel(1);

    OutputController output_controller(file_info.channels, file_info.samplerate);

    vector<double> canal1;
    vector<double> canal2;

    output_controller.play();


    while (input_controller.read_file()) {
        canal1 = filter.convolve(*input_channel0);
        canal2 = filter.convolve(*input_channel1);

        std::cout << output_controller.buffers.size() << " a\n";
        for (int i=0; i < BUFFER_SIZE; ++i) {
            test_audio[i*2] = static_cast<sf::Int16>(canal1[i]);
            test_audio[i*2+1] = static_cast<sf::Int16>(canal2[i]);
        }
        output_controller.appendBuffer(test_audio);
    }

    std::cout << "foi o arquivo.\n";

    while (output_controller.getStatus() == OutputController::Playing) {
        // sf::sleep(sf::seconds(1));
    }


    input_controller.close_file();



    return 0;
}
