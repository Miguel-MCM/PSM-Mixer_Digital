#include <iostream>
#include "./filters.h"
#include "./inputController.h"
#include "./outputController.h"

int main() {
    const string file_input = "./Queen-–-Bohemian-Rhapsody-_Official-Video-Remastered_.wav";
    InputController input_controller;
    input_controller.setFile(file_input);



    vector<sf::Int16> test_audio(BUFFER_SIZE*2);

    vector<double> *input_channel0 = input_controller.getChannel(0);
    vector<double> *input_channel1 = input_controller.getChannel(1);
    // output_controller.set_mutex(&mutex);
    OutputController output_controller(2, 44100);

    input_controller.read_file();
    for (int i=0; i < BUFFER_SIZE; ++i) {
            test_audio[i*2] = static_cast<sf::Int16>(32767.0*(*input_channel0)[i]);
            test_audio[i*2+1] = static_cast<sf::Int16>(32767.0*(*input_channel1)[i]);
        }

    output_controller.appendBuffer(test_audio);
    output_controller.play();


    while (input_controller.read_file()) {
        // std::cout << "a\n";
        for (int i=0; i < BUFFER_SIZE; ++i) {
            test_audio[i*2] = static_cast<sf::Int16>(32767.0*(*input_channel0)[i]);
            test_audio[i*2+1] = static_cast<sf::Int16>(32767.0*(*input_channel1)[i]);
        }
        output_controller.appendBuffer(test_audio);
    }
    while (output_controller.getStatus() == OutputController::Playing) {
        sf::sleep(sf::seconds(1));
    }


    input_controller.close_file();



    return 0;
}
