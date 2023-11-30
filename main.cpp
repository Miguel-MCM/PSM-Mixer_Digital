#include <iostream>
#include <cmath>
#include "./filters.h"
#include "./inputController.h"
#include "./outputController.h"

int main() {
    const string file_input = "./Queen-–-Bohemian-Rhapsody-_Official-Video-Remastered_.wav";
    InputController input_controller;
    input_controller.setFile(file_input);

    std::mutex mutex;

    OutputController output_controller(BUFFER_SIZE*2, 2, 44100);

    vector<sf::Int16> test_audio(BUFFER_SIZE*2);

    vector<double> *input_channel0 = input_controller.getChannel(0);
    vector<double> *input_channel1 = input_controller.getChannel(1);
    // output_controller.set_mutex(&mutex);


    while (input_controller.read_file()) {
        // while (output_controller.getCurrentSample() >= 2*BUFFER_SIZE) {
        std::lock_guard<std::mutex> lock(mutex);

        output_controller.fillBuffer(&test_audio);
        // }
        for (int i=0; i < BUFFER_SIZE; ++i) {
            test_audio[i*2] = static_cast<sf::Int16>(32767.0*(*input_channel0)[i]);
            test_audio[i*2+1] = static_cast<sf::Int16>(32767.0*(*input_channel1)[i]);
        }

        output_controller.play();
    }

    input_controller.close_file();

    sf::Music music;
    music.openFromFile(file_input);
    // if (!music.openFromStream) {
    //     return -1;
    // }
    music.play();
    while (music.getStatus() == sf::Music::Playing) {}


    return 0;
}
